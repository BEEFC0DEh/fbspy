#include "fbserver.h"

#include <QTcpSocket>
#include <QtConcurrent/QtConcurrent>
#include <QBuffer>
#include <QFuture>
#include <QDebug>

FbServer::FbServer(QTcpServer *parent)
    : QTcpServer(parent)
    , m_fbSocket(nullptr)
    , m_futureWatcher(new QFutureWatcher<int>(this))
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

    if (listen(QHostAddress::Any, 31337)) {
        qInfo("Listening on port 31337.");
    }

//    connect(m_futureWatcher, &QFutureWatcher<int>::newConnection, [this](){
//        if (m_fbSocket) {
//            return;
//        }

//        m_fbSocket = nextPendingConnection();
//        connect(m_fbSocket, &QAbstractSocket::disconnected, [this](){
//            m_fbSocket = nullptr;
//        });
//    });
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
//    const auto &future = QtConcurrent::run([raw, this]() => QByteArray {
//        qDebug() << "Bytes before compression" << raw.size();
//        const QByteArray &compressed = qCompress(raw, 1);
//        qDebug() << "Bytes after compression" << compressed.size();

//        QBuffer tmp;
//        tmp.open(QBuffer::WriteOnly);
//        m_fbDataStream.setDevice(&tmp);
//        m_fbDataStream << m_fbView->virtualSize() << compressed;
//        m_fbDataStream.setDevice(m_fbSocket);
//        m_fbDataStream << tmp.size() << m_fbView->virtualSize() << compressed;
//        qDebug() << "Bytes written" << tmp.size() + sizeof(tmp.size());
//    });
    //    m_futureWatcher->setFuture(future);
}

void FbServer::updatePartialFbData(uint id, int position, int size)
{
    if (!m_fbSocket) {
        return;
    }

    const QByteArray &raw = m_fbView->data();
//    const auto &future = QtConcurrent::run([raw, this]() => QByteArray {
//        qDebug() << "Bytes before compression" << raw.size();
//        const QByteArray &compressed = qCompress(raw, 1);
//        qDebug() << "Bytes after compression" << compressed.size();

//        QBuffer tmp;
//        tmp.open(QBuffer::WriteOnly);
//        m_fbDataStream.setDevice(&tmp);
//        m_fbDataStream << m_fbView->virtualSize() << compressed;
//        m_fbDataStream.setDevice(m_fbSocket);
//        m_fbDataStream << tmp.size() << m_fbView->virtualSize() << compressed;
//        qDebug() << "Bytes written" << tmp.size() + sizeof(tmp.size());
//    });
    //    m_futureWatcher->setFuture(future);
}
