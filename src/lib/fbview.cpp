#include "fbview.h"

#include <QSize>

FbView::FbView(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(spyChanged(FbSpy*)), this->parent(), SIGNAL(spyChanged(FbSpy*)));
}

FbSpy *FbView::spy() const
{
    return m_fbSpy;
}

void FbView::setSpy(FbSpy *spy)
{
    if (m_fbSpy == spy) {
        return;
    }

    disconnect(m_fbSpy, nullptr, this, nullptr);

    m_fbSpy = spy;

    if(m_fbSpy) {
        connect(m_fbSpy, SIGNAL(statusChanged(const QString&)), parent(), SLOT(updateStatus(const QString&)));
        connect(m_fbSpy, SIGNAL(virtualSizeChanged(const QSize&)), parent(), SLOT(updateVirtualSize(const QSize&)));
        connect(m_fbSpy, SIGNAL(frameFnished()), parent(), SLOT(finishFrame()));
        connect(m_fbSpy, SIGNAL(partialDataChanged(uint, int, int)), parent(), SLOT(updatePartialFbData(uint, int, int)));
    }

    emit spyChanged(m_fbSpy);
}

bool FbView::running() const
{
    if (m_fbSpy) {
        return m_fbSpy->running();
    } else {
        return false;
    }
}

bool FbView::fbCaptured() const
{
    if (m_fbSpy) {
        return m_fbSpy->fbCaptured();
    } else {
        return false;
    }
}

QSize FbView::virtualSize() const
{
    if (m_fbSpy) {
        return m_fbSpy->virtualSize();
    } else {
        return QSize{};
    }
}

QString FbView::status() const
{
    if (m_fbSpy) {
        return m_fbSpy->status();
    } else {
        return QStringLiteral("FbSpy isn't attached.");
    }
}

const QByteArray &FbView::data() const
{
    if (m_fbSpy) {
        return m_fbSpy->data();
    } else {
        return QByteArray{};
    }
}
