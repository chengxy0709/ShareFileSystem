#include "server.h"
#include "format_packet.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QHostAddress>
#include <QPair>


Server::Server(QObject *parent, int port)
    :QTcpServer(parent)
{
    QHostAddress ip;
    QString ipstr = "127.0.0.1";
    ip.setAddress(ipstr);
    listen(ip, port);
}

QTcpSocket* Server::find_socket(QString username){
    int i = 0;
    for(i = 0; i < tcp_client_socket_list.count(); ++i){
        if(tcp_client_socket_list.at(i).second == username){
            return tcp_client_socket_list.at(i).first;
        }
    }
    return NULL;
}

void Server::incomingConnection(int socketDescriptor){
    Tcp_Client_Socket *tcp_client_socket = new Tcp_Client_Socket(this);
    connect(tcp_client_socket,SIGNAL(UpdateClients(QString,int)),this,SLOT(UpdateClients(QString,int)));
    connect(tcp_client_socket,SIGNAL(Disconnected(int)),this,SLOT(slotDisconnected(int)));
    connect(tcp_client_socket,SIGNAL(UpdateUserName(QString)),this,SLOT(UpdateUserName(QString)));
    connect(tcp_client_socket,SIGNAL(UpdateUserInfo(QString)),this,SLOT(UpdateUserInfo(QString)));
    connect(tcp_client_socket,SIGNAL(UpdateMeta(QString)),this,SLOT(UpdateMeta(QString)));
    connect(tcp_client_socket,SIGNAL(ReturnMeta(QString)),this,SLOT(ReturnMeta(QString)));
    connect(tcp_client_socket,SIGNAL(DeleteMeta(QString)),this,SLOT(DeleteMeta(QString)));
    connect(tcp_client_socket,SIGNAL(SearchMeta(QString)),this,SLOT(SearchMeta(QString)));
    tcp_client_socket->setSocketDescriptor(socketDescriptor);
    QString name = "Unknown";
    QPair<Tcp_Client_Socket*, QString> pair(tcp_client_socket, name);
    tcp_client_socket_list.append(pair);
}

void Server::UpdateClients(QString msg, int length){
    emit UpdateServer(msg, length, UpdateMsg);
    for(int i = 0; i < tcp_client_socket_list.count(); ++i){
        QTcpSocket *item = tcp_client_socket_list.at(i).first;
        format_packet fmsg(msg, OTHERKIND);
        if(item->write(fmsg.fmpak.toLatin1(),length + 8) != length){ //将消息广播给所有人
            continue;
        }
    }
}

void Server::UpdateUserName(QString msg){
    int i = 0;
    int pos = i;
    while(msg.at(i++) != ';');
    QString username = msg.mid(pos, i - 1);
    pos = i;
    while(msg.at(i++) != ';');
    QString password = msg.mid(pos, i - pos - 1);
    QSqlQuery query(db);
    QTcpSocket *item = tcp_client_socket_list.last().first;

    query.exec("select * from login where Username='" + username + "'");
    if(!query.next()){
        msg = "Server: login error, unknown user name.";
        format_packet fmsg(msg, ERRDCKIND);
        item->write(fmsg.fmpak.toLatin1(),fmsg.fmpak.length());
    }
    else{
        if(query.value(2).toString() != password){
            msg = "Server: login error, password is wrong.";
            format_packet fmsg(msg, ERRDCKIND);
            item->write(fmsg.fmpak.toLatin1(),fmsg.fmpak.length());
        }
        else{
            tcp_client_socket_list.last().second = username;
            emit UpdateServer(username, username.length(), UpdateName);
        }
    }
}

void Server::UpdateUserInfo(QString msg){
    int i = 0;
    int pos = i;
    while(msg.at(i++) != ';');
    QString username = msg.mid(pos, i - 1);
    pos = i;
    while(msg.at(i++) != ';');
    QString password = msg.mid(pos, i - pos - 1);
    QSqlQuery query(db);
    QTcpSocket *item = tcp_client_socket_list.last().first;

    qDebug() << "enter";
    query.exec("select * from login where Username='" + username + "'");
    if(query.next()){
        msg = "Server: sign error, user name exists.";
        format_packet fmsg(msg, SIGNFAILKIND);
        item->write(fmsg.fmpak.toLatin1(),fmsg.fmpak.length());
    }
    else{
        if(!query.exec("insert into login values(0, '" + username +
                   "', '" + password + "')")){
            msg = "Server: sign error";
            format_packet fmsg(msg, SIGNFAILKIND);
            item->write(fmsg.fmpak.toLatin1(),fmsg.fmpak.length());
        }
        else {
            msg = "Server: sign success";
            format_packet fmsg(msg, SIGNSCKIND);
            item->write(fmsg.fmpak.toLatin1(),fmsg.fmpak.length());
        }
    }
}


void Server::UpdateMeta(QString mt){
    QSqlQuery query(db);
    tcp_meta mt_t(mt);
    QString query_str = "insert into Resource values(0, ";

    query_str = query_str + "'" + mt_t.filename + "', ";
    query_str = query_str + QString::number(mt_t.size) + ", ";
    query_str = query_str + "'" + mt_t.filepath + "', ";
    query_str = query_str + "'" + mt_t.owner + "', ";
    query_str = query_str + "'" + mt_t.ip + "', ";
    query_str = query_str + QString::number(mt_t.port) + ")";
    query.exec(query_str);
}

void Server::DeleteMeta(QString msg){
    int i = 0;
    int pos = i;
    QSqlQuery query(db);

    while(msg.at(i++) != ';');
    QString username = msg.mid(pos, i - 1);
    qDebug() << msg;
    for(pos = i; i < msg.length(); ++i){
        if(msg.at(i) == ';'){
            QString filename = msg.mid(pos, i - pos);
            query.exec("delete from Resource where FileName='" + filename
                       + "' AND Owner='" + username + "'");
            pos = i + 1;
        }
    }
}

void Server::ReturnMeta(QString username){
    bool flag = false;
    QSqlQuery query(db);
    QTcpSocket *item = find_socket(username);
    if(item == NULL){
        return;
    }

    query.exec("select * from Resource where Owner='" + username + "'");
    while(query.next())
    {
        tcp_meta mt;
        mt.filename = query.value(1).toString();
        mt.size = query.value(2).toString().toLong();
        mt.filepath = query.value(3).toString();
        mt.owner = query.value(4).toString();
        mt.ip = query.value(5).toString();
        mt.port = query.value(6).toString().toLong();
        QString msg = mt.toString();
        format_packet fmsg(msg, RESKIND);
        while(item->write(fmsg.fmpak.toLatin1(),msg.length() + 8) != msg.length() + 8);
        flag = true;
    }
    if(flag == false){
        QString msg = "";
        format_packet fmsg(msg, RESKIND);
        while(item->write(fmsg.fmpak.toLatin1(),msg.length() + 8) != msg.length() + 8);
    }
}

void Server::SearchMeta(QString msg){
    int i = 0;
    int pos = i;
    while(msg.at(i++) != ';');
    QString username = msg.mid(pos, i - 1);
    pos = i;
    while(msg.at(i++) != ';');
    QString filename = msg.mid(pos, i - pos - 1);

    bool flag = false;
    QSqlQuery query(db);
    QTcpSocket *item = find_socket(username);
    if(item == NULL){
        return;
    }

    query.exec("select * from Resource where FileName='" + filename + "'");
    while(query.next())
    {
        tcp_meta mt;
        mt.filename = query.value(1).toString();
        mt.size = query.value(2).toString().toLong();
        mt.filepath = query.value(3).toString();
        mt.owner = query.value(4).toString();
        mt.ip = query.value(5).toString();
        mt.port = query.value(6).toString().toLong();
        if(find_socket(mt.owner) != NULL){
            mt.online = "on";
        }
        else{
            mt.online = "off";
        }
        QString msg = mt.toString();
        format_packet fmsg(msg, SRHKIND);
        while(item->write(fmsg.fmpak.toLatin1(),msg.length() + 8) != msg.length() + 8);
        flag = true;
    }
    if(flag == false){
        QString msg = "";
        format_packet fmsg(msg, SRHKIND);
        while(item->write(fmsg.fmpak.toLatin1(),msg.length() + 8) != msg.length() + 8);
    }
}

void Server::slotDisconnected(int descriptor){
    int i = 0;
    for(i = 0; i < tcp_client_socket_list.count(); ++i){
        QTcpSocket *item = tcp_client_socket_list.at(i).first;
        if(item->socketDescriptor() == descriptor){
            if(tcp_client_socket_list.at(i).second != "Unknown")
                emit UpdateServer("", i, RemoveName);
            tcp_client_socket_list.removeAt(i);
            break;
        }
    }
}

Server::~Server(){
    for(int i = 0; i < tcp_client_socket_list.count(); ++i){
        delete tcp_client_socket_list.at(i).first;
    }
}
