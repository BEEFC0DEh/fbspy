#include "fbspy.h"

FbSpy::FbSpy(QObject *parent)
    : QObject(parent)
{}

void FbSpy::setVirtualSize(const QSize &virtualSize)
{
    if (m_virtualSize == virtualSize)
        return;

    m_fbData.resize(virtualSize.width() * virtualSize.height() * sizeof(int32_t));
    m_virtualSize = virtualSize;
    emit virtualSizeChanged(m_virtualSize);
}

bool FbSpy::running() const
{
    return m_running;
}

bool FbSpy::fbCaptured() const
{
    return m_fbCaptured;
}

QSize FbSpy::virtualSize() const
{
    return m_virtualSize;
}

QString FbSpy::status() const
{
    return m_status;
}

void FbSpy::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;

    if (running) {
        captureFb();
    } else {
        releaseFb();
        m_fbDataId = 0;
        m_fbDataPosition = 0;
    }

    emit runningChanged(m_running);
}

void FbSpy::clearStatus()
{
    m_status.clear();
}

void FbSpy::appendStatus(const QString &status)
{
    m_status.append(status);
}

int FbSpy::appendData(const QByteArray &data)
{
    int inputSize = data.size();
    int fbDataSize = m_fbData.size();
    if (m_fbDataPosition >= fbDataSize || inputSize == 0) {
        return 0;
    }

    int delta = m_fbDataPosition + inputSize;
    int bytesToCopy;
    if (delta > fbDataSize) {
        bytesToCopy = fbDataSize - m_fbDataPosition;
    } else {
        bytesToCopy = inputSize;
    }

    memcpy(&(m_fbData.data()[m_fbDataPosition]), data.constData(), bytesToCopy);
    emit partialDataChanged(m_fbDataId, m_fbDataPosition, data.size());
    m_fbDataPosition += inputSize;
    return data.size();
}

const QByteArray &FbSpy::data() const
{
    return m_fbData;
}

void FbSpy::finalizeData()
{
    m_fbDataPosition = 0;
    ++m_fbDataId;
    emit frameFinished();
}

void FbSpy::setFbCaptured(bool fbCaptured)
{
    if (m_fbCaptured == fbCaptured)
        return;

    m_fbCaptured = fbCaptured;
    emit fbCapturedChanged(m_fbCaptured);
}
