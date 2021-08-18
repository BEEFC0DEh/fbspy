#ifndef FBSOCKETSPY_H
#define FBSOCKETSPY_H

#include <QPointer>
#include <QDataStream>
#include "fbspy.h"

class QTcpSocket;

class FbSocketSpy : public FbSpy
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)

public:
    explicit FbSocketSpy(QObject *parent = nullptr);

    QString address() const;

public slots:
    void setAddress(QString address);
    void connect();

signals:
    void addressChanged(QString address);

protected:
    virtual void captureFb() override;
    virtual void releaseFb() override;

private:
    QDataStream m_fbDataStream;
    QString m_address;
    QTcpSocket *m_fbSocket;
    qint64 m_bytesToWaitFor;
    qint64 m_bytesReceived;
};

#endif // FBSOCKETSPY_H
