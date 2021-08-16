#ifndef FBSERVER_H
#define FBSERVER_H

#include <QObject>
#include <QTcpServer>
#include "fbview.h"

class QTcpSocket;

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

private:
    QTcpSocket *m_fbSocket;
    FbView *m_fbView;
};

#endif // FBSERVER_H
