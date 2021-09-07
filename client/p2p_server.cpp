#include "p2p_server.h"
#include <QFile>
#include <QFileDialog>

P2P_Server::P2P_Server(QObject *parent, int port)
    :QTcpServer(parent)
{
    tcp_client_socket = NULL;
    listen(QHostAddress::Any, port);
}

void P2P_Server::incomingConnection(int socketDescriptor){
    tcp_client_socket = new QTcpSocket(this);
    connect(tcp_client_socket,SIGNAL(readyRead()),this,SLOT(checkdata()));
    connect(tcp_client_socket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    tcp_client_socket->setSocketDescriptor(socketDescriptor);
    qDebug() << "connected";
}

//#len#filepath#
#define PAK_ERR "PKE"
#define FP_ERR  "FPE"
#define SUCC    "SUC"
void P2P_Server::checkdata(){

    if(tcp_client_socket->bytesAvailable() >= 11){

        char buf[1024];
        tcp_client_socket->read(buf,11);
        QString msg;

        if(buf[0] != '#'){
            msg = PAK_ERR;
            tcp_client_socket->write(msg.toLatin1(), msg.length());
            return;
        }

        msg = buf;
        long len = msg.mid(1, 10).toLong();
        tcp_client_socket->read(buf,len + 2);

        msg = buf;
        QString filepath = msg.mid(1, len);
        transfer(filepath);
    }

}

void P2P_Server::transfer(QString& filepath){
    QFile file(filepath);
    QFileInfo info(file);
    qint64 filesize = info.size();
    QString msg;

    if(file.open(QIODevice::ReadOnly)){
        char buf[257];
        msg = SUCC;
        tcp_client_socket->write(msg.toLatin1(), msg.length());
        for(int i = 0; i < filesize / 256; ++i) {
            file.read(buf, 256);
            qDebug() << tcp_client_socket->write(buf, 256);
            qDebug() << tcp_client_socket->waitForBytesWritten(3000);
        }
        file.read(buf, filesize % 256);
        qDebug() << tcp_client_socket->write(buf, filesize % 256);
        qDebug() <<  tcp_client_socket->waitForBytesWritten(3000);
    }
    else{
        msg = FP_ERR;
        tcp_client_socket->write(msg.toLatin1(), msg.length());
    }
}

void P2P_Server::slotDisconnected(){

}

P2P_Server::~P2P_Server(){
    if(tcp_client_socket != NULL)
        delete tcp_client_socket;
}

