#ifndef FBSERVER_H
#define FBSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDataStream>
#include <QFutureWatcher>
#include "fbview.h"

class QTcpSocket;
//class QFutureWatcher;

class FbServer : public QTcpServer
{
    Q_OBJECT

    Q_PROPERTY(FbSpy *spy READ spy WRITE setSpy NOTIFY spyChanged)
public:
    explicit FbServer(QTcpServer *parent = nullptr);

    FbSpy *spy() const;

public slots:
    void setSpy(FbSpy * spy);

signals:
    void spyChanged(FbSpy * spy);

private slots:
    void updateFbData();
    void updatePartialFbData(uint id, int position, int size);

private:
    QDataStream m_fbDataStream;
    QTcpSocket *m_fbSocket;
    QFutureWatcher<int> *m_futureWatcher;
    FbView *m_fbView;
};

#endif // FBSERVER_H
