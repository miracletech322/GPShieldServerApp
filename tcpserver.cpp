#include "tcpserver.h"
#include "tcpsocket.h"
#include "mainwindow.h"
#include <QMessageBox>
#include "global_vars.h"

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
{

}

void TcpServer::handleProcess(int nHeader, QString strIp, QString strName, TcpSocket* pSocket)
{
    QMutexLocker locker(&m_mutex);
    if(nHeader == RequestHeader::RH_SET_NAME) {
        MainWindow::getInstance()->handleProcessStatus(HandleProcessStatus::HPS_PEER_SET_INFO, tr("%1(%2)").arg(strName).arg(strIp));
    }

    if(nHeader == RequestHeader::RH_SEND_ALERT) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_5);

        out << nHeader;
        out << strIp;
        out << strName;

        int nLength = m_lstSocket.count();

        for(int i = 0; i < nLength; i++) {
            if(m_lstSocket[i]->peerAddress().toString() == strIp && m_lstSocket[i]->peerName() == strName)
                continue;
            if (m_lstSocket[i] && m_lstSocket[i]->isOpen()) {
                m_lstSocket[i]->write(block);
            }
        }

        MainWindow::getInstance()->handleProcessStatus(HandleProcessStatus::HPS_PEER_SEND, tr("%1(%2)").arg(strName).arg(strIp));
    }

    if(nHeader == RequestHeader::RH_RECEIVED_ALERT) {
        int nLength = m_lstSocket.count();

        for(int i = 0; i < nLength; i++) {
            if(m_lstSocket[i]->peerAddress().toString() == strIp && m_lstSocket[i]->peerName() == strName) {
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_6_5);

                QString strMyIp = pSocket->peerAddress().toString();
                QString strMyName = pSocket->peerName();

                out << nHeader;
                out << strMyIp;
                out << strMyName;
                m_lstSocket[i]->write(block);

                MainWindow::getInstance()->handleProcessStatus(HandleProcessStatus::HPS_PEER_RECEIVED, tr("The %1(%2) has comfirmed %3(%4)'s alert.").arg(strMyName).arg(strMyIp).arg(strName).arg(strIp));
                break;
            }
        }
    }
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
        QString strMessage = tr("%1(%2)").arg(pSocket->peerName()).arg(pSocket->peerAddress().toString());
        MainWindow::getInstance()->handleProcessStatus(HandleProcessStatus::HPS_PEER_CLOSED, strMessage);

        pSocket = nullptr;
        delete pSocket;
    }
}
