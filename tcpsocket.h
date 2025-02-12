#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
class TcpServer;

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(int socketDescriptor, QObject *parent = nullptr);

signals:
    void sig_error(TcpSocket*);

public slots:
    void slt_socketError(QAbstractSocket::SocketError socketError);
    void slt_readyRead();
    void slt_disconnected();

private:
    TcpServer* m_pServer;
};

#endif // TCPSOCKET_H
