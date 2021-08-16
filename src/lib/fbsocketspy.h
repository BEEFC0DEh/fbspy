#ifndef FBSOCKETSPY_H
#define FBSOCKETSPY_H

#include <QPointer>
#include "fbspy.h"

class QTcpSocket;

class FbSocketSpy : public FbSpy
{
    Q_OBJECT
public:
    explicit FbSocketSpy(QObject *parent = nullptr);

private:
    QPointer<QTcpSocket> m_fbSocket;
};

#endif // FBSOCKETSPY_H
