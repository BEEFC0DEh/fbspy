#include "fbspy.h"

FbSpy::FbSpy(QObject *parent)
    : QObject(parent)
{}

void FbSpy::setVirtualSize(const QSize &virtualSize)
{
    if (m_virtualSize == virtualSize)
        return;

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

    if (running) {
        captureFb();
    } else {
        releaseFb();
    }

    m_running = running;
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
    m_tmpFbData += data;
    return data.size();
}

const QByteArray &FbSpy::data() const
{
    return m_fbData;
}

void FbSpy::finalizeData()
{
    m_fbData.swap(m_tmpFbData);
    m_tmpFbData.clear();
    emit dataChanged();
}

void FbSpy::setFbCaptured(bool fbCaptured)
{
    if (m_fbCaptured == fbCaptured)
        return;

    m_fbCaptured = fbCaptured;
    emit fbCapturedChanged(m_fbCaptured);
}
