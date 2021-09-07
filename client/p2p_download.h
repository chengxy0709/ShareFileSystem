#ifndef P2P_DOWNLOAD
#define P2P_DOWNLOAD

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include "tcp_meta.h"

class P2P_Download : public QTcpSocket
{
    Q_OBJECT

public:
    P2P_Download(QObject *parent, tcp_meta& tm);
    ~P2P_Download();

    qint64 progress;
    bool status;

    bool start_as_client(QString dirname);

private:
    int port;
    QHostAddress *ServerIP;
    QString UserName;
    QString Dirname;
    QString Dfilepath;
    QString Dfilename;
    qint64 filesize;

private slots:
    void ClientConnected();
    void ClientDisconnected();
    void ClientDataReceived();

};

#endif // P2P_DOWNLOAD

