#include "tcp_client.h"
#include <QApplication>
#include "tcp_meta.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
