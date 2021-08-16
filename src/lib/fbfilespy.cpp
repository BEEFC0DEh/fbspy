#include "fbfilespy.h"
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QMutexLocker>

static const int TIMER_INTERVAL = 30;

FbFileSpy::FbFileSpy(QObject *parent)
    : FbSpy(parent)
    , m_fbFile(new QFile{this})
{
    m_fbFile->setFileName(QStringLiteral("/dev/fb0"));
}

void FbFileSpy::captureFb()
{
    clearStatus();

    if (m_fbFile->open(QIODevice::ReadOnly)) {
        QFile fbSize(QStringLiteral("/sys/class/graphics/fb0/virtual_size"));

        if (!fbSize.open(QIODevice::ReadOnly)) {
            appendStatus(QLatin1Literal("Couldn't read fb0 size.\n"));

        } else {
            const QStringList &size = QString::fromLatin1(fbSize.readAll()).split(',');
            setVirtualSize(QSize(size.at(0).toInt(), size.at(1).toInt()));
            setFbCaptured(true);
            m_timer = startTimer(TIMER_INTERVAL);
        }
    } else {
        appendStatus(QLatin1Literal("Couldn't open fb0.\n"));
        m_fbFile->close();
    }

    emit statusChanged(status());
}

void FbFileSpy::releaseFb()
{
    m_fbFile->close();
    killTimer(m_timer);
    setVirtualSize(QSize{});
    setFbCaptured(false);
    setRunning(false);
}

void FbFileSpy::readFb()
{
    if (!fbCaptured() || !running()) {
        return;
    }

    m_elapsedTimer.restart();

    if (m_fbFile->isOpen()) {
        int bytesRead = appendData(m_fbFile->read(m_bytesToRead));
//        qInfo() << "Read" << bytesRead << "bytes.";
        m_totalReadBytes += bytesRead;
        ++m_byteSamples;
        if (bytesRead < m_bytesToRead) {
            m_fbFile->close();

            finalizeData();

            m_totalReadTime += m_elapsedTimer.elapsed();
            ++m_timeSamples;
            qInfo() << "Total read bytes:" << m_totalReadBytes << "Avg.:" << m_totalReadBytes / m_byteSamples;
            qInfo() << "Total read time:" << m_totalReadTime << "Avg.:" << m_totalReadTime / m_timeSamples;
            m_totalReadBytes = 0;
            m_totalReadTime = 0;
            m_byteSamples = 0;
            m_timeSamples = 0;

            if (!m_fbFile->open(QIODevice::ReadOnly)) {
                releaseFb();
            }

            return;
        }

    } else {
        if (!m_fbFile->open(QIODevice::ReadOnly)) {
            releaseFb();
        }
    }

    m_totalReadTime += m_elapsedTimer.elapsed();
    ++m_timeSamples;

    // Try to minimise CPU usage to stay undetected.
    if (m_elapsedTimer.elapsed() > TIMER_INTERVAL / 3) {
        m_bytesToRead /= 2;

    } else {
        static const int ur2slow = 8 * 1024;
        if (m_bytesToRead < ur2slow) {
            m_bytesToRead += ur2slow;
        } else {
            m_bytesToRead *= 1.5;
        }

        static const int tenMiB = 10 * 1024 * 1024;
        if (m_bytesToRead > tenMiB) {
            m_bytesToRead = tenMiB;
        }
    }
}

void FbFileSpy::timerEvent(QTimerEvent */*event*/)
{
    readFb();
}
