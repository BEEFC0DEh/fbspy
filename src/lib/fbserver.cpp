#include "fbserver.h"

#include <QTcpSocket>
#include <QDebug>

FbServer::FbServer(QTcpServer *parent)
    : QTcpServer(parent)
    , m_fbSocket(nullptr)
    , m_fbView(new FbView(this))
{
    connect(this, &QTcpServer::newConnection, [this](){
        if (m_fbSocket) {
            return;
        }

        m_fbSocket = nextPendingConnection();
        connect(m_fbSocket, &QAbstractSocket::disconnected, [this](){
            m_fbSocket = nullptr;
        });
    });
}

FbSpy *FbServer::spy() const
{
    return m_fbView->spy();
}

void FbServer::setSpy(FbSpy *spy)
{
    m_fbView->setSpy(spy);
}

void FbServer::updateFbData()
{
    if (!m_fbSocket) {
        return;
    }

    const QByteArray &raw = m_fbView->data();
    qDebug() << "Bytes before compression" << raw.size();
    const QByteArray &compressed = qCompress(raw);
    qDebug() << "Bytes after compression" << compressed.size();
    qDebug() << "Bytes written" << m_fbSocket->write(compressed);
}
