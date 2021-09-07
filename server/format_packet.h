#ifndef FORMAT_PACKET
#define FORMAT_PACKET

#include <QString>

enum fkind{
    RESKIND = 0,
    SRHKIND = 1,
    ERRDCKIND = 2,
    SIGNSCKIND = 3,
    SIGNFAILKIND = 4,
    OTHERKIND = 5,
};

struct format_packet{
    format_packet(QString &str, fkind);
    QString fmpak;
};

#endif // FORMAT_PACKET

