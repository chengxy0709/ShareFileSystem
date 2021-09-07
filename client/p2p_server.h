#ifndef P2P_SERVER
#define P2P_SERVER

#include <QTcpServer>
#include <QObject>
#include <QString>
#include <QTcpSocket>

#include "tcp_meta.h"

class P2P_Server : public QTcpServer
{
    Q_OBJECT
public:
    P2P_Server(QObject *parent = 0, int port = 0);
    ~P2P_Server();

private slots:
    void checkdata();
    void slotDisconnected();

protected:
    void incomingConnection(int socketDescriptor);

private:
    QTcpSocket *tcp_client_socket;
    void transfer(QString& filepath);

};

#endif // P2P_SERVER

