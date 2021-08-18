#include "fbsocketspy.h"
#include <QTcpSocket>
#include <QDebug>

FbSocketSpy::FbSocketSpy(QObject *parent)
    : FbSpy(parent)
    , m_fbSocket(new QTcpSocket(this))
    , m_bytesToWaitFor(0)
    , m_bytesReceived(0)
{
    QObject::connect(m_fbSocket, &QTcpSocket::connected, [](){
        qInfo("Connected to a FbServer.");
    });

    QObject::connect(m_fbSocket, &QTcpSocket::disconnected, [this](){
        m_fbDataStream.abortTransaction();
        qInfo("Disconnected from the FbServer.");
    });

    m_fbDataStream.setDevice(m_fbSocket);
    QObject::connect(m_fbSocket, &QTcpSocket::readyRead, [this](){
        static const int HEADER = 0;
        static const int DATA = 1;
        static       int streamState = HEADER;
        m_bytesReceived += m_fbSocket->bytesAvailable();
        qInfo() << "Received" << m_fbSocket->bytesAvailable() << "bytes.";

        while (true) {
            switch (streamState) {
            case HEADER:
                if (m_bytesReceived < sizeof(m_bytesToWaitFor)) {
                    return;
                }

                m_fbDataStream >> m_bytesToWaitFor;
                qInfo() << "Received header," << m_bytesToWaitFor << "bytes to read.";
                streamState = DATA;
                break;

            case DATA:
                if (m_bytesReceived < m_bytesToWaitFor) {
                    return;
                }

                {
                    QSize virtualSize;
                    QByteArray data;
                    m_fbDataStream >> virtualSize >> data;
                    qInfo() << "Received" << data.size() << "bytes of data.";
                    setVirtualSize(virtualSize);
                    appendData(data);
                    finalizeData();
                }

                streamState = HEADER;
                m_bytesReceived = m_fbSocket->bytesAvailable();
                break;

            default:
                break;
            }
        }
    });
}

QString FbSocketSpy::address() const
{
    return m_address;
}

void FbSocketSpy::setAddress(QString address)
{
    if (m_address == address)
        return;

    m_address = address;
    emit addressChanged(m_address);
}

void FbSocketSpy::connect()
{
    captureFb();
}

void FbSocketSpy::captureFb()
{
    m_fbSocket->connectToHost(m_address, 31337);
}

void FbSocketSpy::releaseFb()
{
    m_fbSocket->disconnectFromHost();
}
