#ifndef SERVER
#define SERVER

#include <QTcpServer>
#include <QObject>
#include <QList>
#include <QString>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "tcp_client_socket.h"
#include "tcp_meta.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0, int port = 0);
    ~Server();

    QList<QPair<Tcp_Client_Socket*, QString> > tcp_client_socket_list;

    QSqlDatabase db;

    enum MsgKind{
        UpdateName = 0,
        UPDATEMETA = 1,
        UpdateMsg = 2,
        RemoveName = 3,
    };

signals:
    void UpdateServer(QString, int, Server::MsgKind);
private slots:
    QTcpSocket* find_socket(QString);

    void UpdateClients(QString, int);
    void slotDisconnected(int);
    void UpdateUserName(QString);
    void UpdateUserInfo(QString);
    void UpdateMeta(QString);
    void ReturnMeta(QString);
    void DeleteMeta(QString);
    void SearchMeta(QString);

protected:
    void incomingConnection(int socketDescriptor);

};

#endif // SERVER

