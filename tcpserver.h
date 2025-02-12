#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QByteArray>

class TcpSocket;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = nullptr);

    void handleProcess(QByteArray bytes);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public slots:
    void slt_socketError(TcpSocket* pSocket);

private:
    QList<TcpSocket*> m_lstSocket;
};

#endif // TCPSERVER_H
