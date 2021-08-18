#include "fbviewitem.h"

#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QElapsedTimer>

#include "fbview.h"

static QElapsedTimer updateTimer;

FbViewItem::FbViewItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_fbImage(textureSize(), QImage::Format_ARGB32)
    , m_currentFbDataId(0)
    , m_fbView(new FbView(this))
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    connect(this, &QQuickPaintedItem::textureSizeChanged, [this](){
        qInfo() << "Texture size changed to" << textureSize();
        setSize(textureSize());
        m_fbImage = QImage{textureSize(), QImage::Format_ARGB32};
        updateFbData();
    });

    setTextureSize(QSize{1000, 1000});
    updateTimer.start();
}

void FbViewItem::copyFbData(int start, int size)
{
    QElapsedTimer timer;
    timer.start();
    int imageByteCount = m_fbImage.byteCount();
    if (start >= imageByteCount || size == 0) {
        return;
    }

    const auto &data =  m_fbView->data();
    int delta = start + size;
    int bytesToCopy;
    if (delta > imageByteCount) {
        bytesToCopy = imageByteCount - start;
    } else {
        bytesToCopy = size;
    }

    memcpy(&(m_fbImage.bits()[start]), &(data.constData()[start]), bytesToCopy);
    qInfo() << "Copied" << bytesToCopy << "bytes to the texture.";
    qInfo() << "It took" << timer.elapsed() << "ms.";
}

void FbViewItem::updateFbData()
{
    if (!m_fbView->spy()) {
        return;
    }

    copyFbData(0, m_fbView->data().size());
    update();
}

void FbViewItem::updatePartialFbData(uint id, int position, int size)
{
    copyFbData(position, size);

    if (updateTimer.elapsed() < 500) {
        return;
    }

    updateTimer.restart();
    update();
}

void FbViewItem::updateVirtualSize(const QSize &virtualSize)
{
    setTextureSize(virtualSize);
}

void FbViewItem::paint(QPainter *painter)
{
    QElapsedTimer timer;
    timer.start();
    if (m_fbView->fbCaptured() && m_fbImage.valid(0, 0)) {
        qInfo() << "Image size:" << m_fbImage.size();
        painter->drawImage(0, 0, m_fbImage);

    } else {
        QBrush brush{Qt::red, Qt::BDiagPattern};
        const int width = textureSize().width();
        const int height = textureSize().height();
        painter->setBrush(brush);
        painter->drawRect(0, 0, width, height);
        painter->setPen(Qt::white);
        painter->drawText(boundingRect(), m_fbView->status());
    }
    qInfo() << "Drawing took" << timer.elapsed() << "ms.";
}

FbSpy *FbViewItem::spy() const
{
    return m_fbView->spy();
}

void FbViewItem::setSpy(FbSpy *spy)
{
    m_fbView->setSpy(spy);
}
