#include "tcp_client_socket.h"
#include <QRegExp>
#include <QByteArray>

Tcp_Client_Socket::Tcp_Client_Socket(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(DataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void Tcp_Client_Socket::DataReceived(){
    if(bytesAvailable() > 0) {
        char buf[1024];
        read(buf, 8);

        QString head = buf;
        long msglen = 0;
        QString kind;
        if(head == "error|||"){
            return;
        }
        else{
            msglen = head.mid(2,6).toLong();
            kind = head.mid(0,2);
            read(buf,msglen);
            buf[msglen] = 0;
        }

        QString msg = buf;
        if(kind == "NM"){ //name messsage
            QString logininfo = msg;
            emit UpdateUserName(logininfo);
        }
        else if(kind == "MM"){ //meta message
            emit UpdateMeta(msg);
        }
        else if(kind == "SM"){ //return share file message
            QString username = msg;
            emit ReturnMeta(username);
        }
        else if(kind == "DM"){ //delete share file message
            emit DeleteMeta(msg);
        }
        else if(kind == "SR"){ //search share file message
            emit SearchMeta(msg);
        }
        else if(kind == "DC"){ //disconnected message
        }
        else if(kind == "GM"){ //sign message
            emit UpdateUserInfo(msg);
        }
        else if(kind == "OT"){ //normal message
            emit UpdateClients(msg, msg.length());
        }
    }
}

void Tcp_Client_Socket::slotDisconnected(){
    emit Disconnected(this->socketDescriptor());
}

Tcp_Client_Socket::~Tcp_Client_Socket(){

}
