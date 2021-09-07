#ifndef TCP_META
#define TCP_META

#include <QString>

struct tcp_meta{
    tcp_meta(){}
    tcp_meta(QString);

    QString filename;
    qint64 size;
    QString owner;
    QString ip;
    qint64 port;
    QString filepath;
    QString online;

    bool status;

    QString toString();
};

#endif // META

