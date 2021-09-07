#include "tcp_meta.h"
#include <QVector>

// #S#FN:filename;SZ:size;OW:owner;IP:ip;PT:port#E#
tcp_meta::tcp_meta(QString mt){
    QString head = mt.mid(0,3);
    if(head != "#S#"){
        status = false;
        return;
    }
    int i = 3, k = i;
    QVector<QString> strs;
    strs.push_back("FN:");strs.push_back("SZ:");strs.push_back("OW:");
    strs.push_back("IP:");strs.push_back("PT:");strs.push_back("FP:");
    strs.push_back("OL:");
    for(int cnt = 0; cnt < 7; ++cnt){
        head = mt.mid(i,3);
        if(head != strs.at(cnt)){
            status = false;
            return;
        }
        else {
            i = i + 3;
            k = i;
            while(mt.at(i++) != ';');
            strs[cnt] = mt.mid(k, i - k - 1);
            k = i;
        }
    }
    filename = strs.at(0);
    size = strs.at(1).toLong();
    owner = strs.at(2);
    ip = strs.at(3);
    port = strs.at(4).toLong();
    filepath = strs.at(5);
    online = strs.at(6);

    status = true;
}

QString tcp_meta::toString(){
    QString res;
    res = res + "#S#";
    res = res + "FN:" + filename + ";";
    res = res + "SZ:" + QString::number(size) + ";";
    res = res + "OW:" + owner + ";";
    res = res + "IP:" + ip + ";";
    res = res + "PT:" + QString::number(port) + ";";
    res = res + "FP:" + filepath + ";";
    res = res + "OL:" + online + ";";
    res = res + "#E#";
    return res;
}
