#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QByteArray>
#include <QMutex>

class TcpSocket;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = nullptr);

    void handleProcess(int nHeader, QString strIp, QString strName, TcpSocket* pSocket = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public slots:
    void slt_socketError(TcpSocket* pSocket);

private:
    QList<TcpSocket*> m_lstSocket;
    QMutex m_mutex;
};

#endif // TCPSERVER_H
