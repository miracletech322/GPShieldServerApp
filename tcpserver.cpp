#include "tcpserver.h"
#include "tcpsocket.h"
#include "mainwindow.h"
#include <QMessageBox>
#include "global_vars.h"

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
{

}

void TcpServer::handleProcess(QByteArray bytes)
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    MainWindow::getInstance()->handleProcessStatus(HandleProcessStatus::HPS_PEER_CONNECTED);

    TcpSocket* pSocket = new TcpSocket(socketDescriptor, this);
    connect(pSocket, SIGNAL(sig_error(TcpSocket*)), this, SLOT(slt_socketError(TcpSocket*)));

    m_lstSocket << pSocket;
}

void TcpServer::slt_socketError(TcpSocket* pSocket)
{
    if(m_lstSocket.removeOne(pSocket))
    {
        MainWindow::getInstance()->handleProcessStatus(HandleProcessStatus::HPS_PEER_CLOSED);

        pSocket = nullptr;
        delete pSocket;
    }
}
