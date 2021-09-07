#ifndef TCP_CLIENT_SOCKET
#define TCP_CLIENT_SOCKET

#include <QTcpSocket>
#include <QObject>
#include <QString>

class Tcp_Client_Socket : public QTcpSocket
{
    Q_OBJECT
public:
    Tcp_Client_Socket(QObject *parent = 0);
    ~Tcp_Client_Socket();

signals:
    void UpdateClients(QString, int);
    void UpdateUserName(QString);
    void UpdateMeta(QString);
    void Disconnected(int);
    void ReturnMeta(QString);
    void DeleteMeta(QString);
    void SearchMeta(QString);
    void UpdateUserInfo(QString);
protected slots:
    void DataReceived();
    void slotDisconnected();
};

#endif // TCP_CLIENT_SOCKET

