#include "p2p_download.h"
#include <QFile>

P2P_Download::P2P_Download(QObject *parent, tcp_meta &tm)
    :QTcpSocket(parent)
{
    status = true;
    port = tm.port + 1;
    ServerIP = new QHostAddress;
    ServerIP->setAddress(tm.ip);
    UserName = tm.owner;
    Dfilepath = tm.filepath;
    Dfilename = tm.filename;
    filesize = tm.size;
}

bool P2P_Download::start_as_client(QString dirname){
    if(dirname == ""){
        return false;
    }
    Dirname = dirname;
    connect(this,SIGNAL(connected()),this,SLOT(ClientConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
    connect(this,SIGNAL(readyRead()),this,SLOT(ClientDataReceived()));

    connectToHost(*ServerIP,port);
    return true;
}

void P2P_Download::ClientConnected(){

    QString msg = "#";
    QString slen = QString::number(Dfilepath.length());
    for(int i = slen.length(); i < 10; ++i){
        slen = "0" + slen;
    }
    msg = msg + slen + "#" + Dfilepath + "#";
    write(msg.toLatin1(), msg.length());
}



#define PAK_ERR "PKE"
#define FP_ERR  "FPE"
#define SUCC    "SUC"
void P2P_Download::ClientDataReceived(){
    if(bytesAvailable() >= 3){
        char buf[256];

        read(buf, 3);
        QString msg = buf;
        if(msg.mid(0,3) == PAK_ERR){

        }
        else if(msg.mid(0,3) == FP_ERR){

        }
        else if(msg.mid(0,3) == SUCC){
            QFile dfile(Dirname + Dfilename);
            qint64 tsz = 0;
            if(dfile.open(QIODevice::ReadWrite)){
                for(int i = 0; i < filesize / 256; ++i){
                    while(bytesAvailable() < 256){ //block
                        if(!waitForReadyRead(3000)){
                            qDebug() << "timeout";
                            disconnectFromHost();
                            status = false;
                            return;
                        }
                    }
                    read(buf, 256);
                    dfile.write(buf, 256);
                    tsz = tsz + 256;
                    progress = tsz * 100 / filesize;
                    qDebug() << progress;
                }
                while(bytesAvailable() < (filesize % 256)){ //block
                    if(!waitForReadyRead(3000)){
                        qDebug() << "timeout";
                        disconnectFromHost();
                        status = false;
                        return;
                    }
                }
                read(buf, filesize % 256);
                dfile.write(buf, filesize % 256);
                progress = 100;
                qDebug() << progress;
            }
            status = true;
            disconnectFromHost();
        }
    }
}

void P2P_Download::ClientDisconnected(){

}

P2P_Download::~P2P_Download(){
    this->close();
}
