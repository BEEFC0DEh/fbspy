#ifndef FBSPY_H
#define FBSPY_H

#include <QObject>
#include <QSize>

class FbSpy : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool fbCaptured READ fbCaptured NOTIFY fbCapturedChanged)
    Q_PROPERTY(QSize virtualSize READ virtualSize NOTIFY virtualSizeChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit FbSpy(QObject *parent = nullptr);

    bool running() const;
    bool fbCaptured() const;
    QSize virtualSize() const;
    QString status() const;

public slots:
    void setRunning(bool running);

signals:
    void runningChanged(bool running);
    void fbCapturedChanged(bool fbCaptured);
    void virtualSizeChanged(const QSize &virtualSize);
    void statusChanged(const QString &status);
    void dataChanged();

protected:
    virtual void captureFb() = 0;
    virtual void releaseFb() = 0;

    void setFbCaptured(bool fbCaptured);
    void setVirtualSize(const QSize &virtualSize);

    void clearStatus();
    void appendStatus(const QString &status);

    int appendData(const QByteArray &data);
    void finalizeData();

private:
    const QByteArray &data() const;

    QByteArray m_tmpFbData;
    QByteArray m_fbData;
    QString m_status;
    QSize m_virtualSize;
    bool m_running = false;
    bool m_fbCaptured = false;

    friend class FbView;
};

#endif // FBSPY_H
