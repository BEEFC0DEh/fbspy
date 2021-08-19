#ifndef FBFILESPY_H
#define FBFILESPY_H

#include <QObject>
#include <QElapsedTimer>

#include "fbspy.h"

class QFile;

class FbFileSpy : public FbSpy
{
    Q_OBJECT

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;

public:
    explicit FbFileSpy(QObject *parent = nullptr);
    virtual  ~FbFileSpy() override;

protected:
    virtual void captureFb() override;
    virtual void releaseFb() override;

private:
    void readFb();

    QElapsedTimer m_elapsedTimer;
    int m_timer = 0;
    int m_totalReadTime = 0;
    int m_timeSamples = 0;
    int m_totalReadBytes = 0;
    int m_byteSamples = 0;
    qint64 m_bytesToRead = 640 * 1024;
    uchar *m_fbMap = nullptr;
    QFile *m_fbFile;
};

#endif // FBFILESPY_H
