#include "tcpsocket.h"
#include "tcpserver.h"
#include "global_vars.h"

TcpSocket::TcpSocket(int socketDescriptor, QObject *parent)
    : QTcpSocket{parent}
{
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
    in >> nHeader;

    if(nHeader == RequestHeader::RH_SET_NAME)
    {
        QString strName;
        in >> strName;
        setPeerName(strName);
    }

    in.commitTransaction();
}

void TcpSocket::slt_disconnected()
{
    emit sig_error(this);
}
