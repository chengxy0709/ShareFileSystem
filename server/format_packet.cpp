#include "format_packet.h"

format_packet::format_packet(QString &str, fkind kind){
    long len = str.length();
    QString s = QString::number(len);
    if(len > 100000){
        str = "error|||";
        return;
    }
    for(int i = s.length(); i < 6; ++i){
        s = "0" + s;
    }
    switch (kind) {
    case RESKIND:
        fmpak = "RK" + s + str;
        break;
    case SRHKIND:
        fmpak = "SR" + s + str;
        break;
    case ERRDCKIND:
        fmpak = "EK" + s + str;
        break;
    case SIGNSCKIND:
        fmpak = "SS" + s + str;
        break;
    case SIGNFAILKIND:
        fmpak = "SF" + s + str;
        break;
    case OTHERKIND:
        fmpak = "OT" + s + str;
        break;
    }
}

