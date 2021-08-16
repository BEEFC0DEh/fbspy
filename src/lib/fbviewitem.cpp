#include "fbviewitem.h"

#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QElapsedTimer>

#include "fbview.h"

FbViewItem::FbViewItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_fbImage(textureSize(), QImage::Format_ARGB32)
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
}

void FbViewItem::updateFbData()
{
    QElapsedTimer timer;
    timer.start();
    if (!m_fbView->spy()) {
        return;
    }

    const auto &data =  m_fbView->data();
    int bytesToCopy;
    if (data.size() > m_fbImage.byteCount()) {
        bytesToCopy = m_fbImage.byteCount();
    } else {
        bytesToCopy = data.size();
    }

    memcpy(m_fbImage.bits(), data.constData(), bytesToCopy);
    qInfo() << "Copied" << bytesToCopy << "bytes to the texture.";
    qInfo() << "It took" << timer.elapsed() << "ms.";
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
