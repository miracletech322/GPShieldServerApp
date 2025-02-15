#include "tcpsocket.h"
#include "tcpserver.h"
#include "global_vars.h"

TcpSocket::TcpSocket(int socketDescriptor, QObject *parent)
    : QTcpSocket{parent}
{
    m_pServer = (TcpServer*)parent;
    if(!setSocketDescriptor(socketDescriptor))
    {
        emit sig_error(this);
    }
    else
    {
        connect(this, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(slt_socketError(QAbstractSocket::SocketError)));
        connect(this, SIGNAL(readyRead()), this, SLOT(slt_readyRead()));
        connect(this, SIGNAL(disconnected()), this, SLOT(slt_disconnected()));
    }
}

void TcpSocket::slt_socketError(SocketError)
{
    emit sig_error(this);
}

void TcpSocket::slt_readyRead()
{
    QDataStream in;
    in.setDevice(this);
    in.setVersion(QDataStream::Qt_6_5);
    in.startTransaction();

    int nHeader;
    QString strIp;
    QString strName;
    QString strDepart;

    in >> nHeader;
    in >> strIp;
    in >> strName;
    in >> strDepart;

    in.commitTransaction();

    if(nHeader == RequestHeader::RH_SET_NAME)
    {
        setPeerAddress(QHostAddress(strIp));
        setPeerName(strName);
        m_strDepart = strDepart;

        m_pServer->handleProcess(nHeader, strIp, strDepart, strName);
    }

    if(nHeader == RequestHeader::RH_SEND_ALERT)
    {
        m_pServer->handleProcess(nHeader, strIp, strDepart, strName);
    }

    if(nHeader == RequestHeader::RH_RECEIVED_ALERT)
    {
        m_pServer->handleProcess(nHeader, strIp, strDepart, strName, this);
    }
}

void TcpSocket::slt_disconnected()
{
    emit sig_error(this);
}
