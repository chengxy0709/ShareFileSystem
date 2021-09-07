#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QTreeWidgetItem>
#include <QString>

#include "server.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotCreateServer();
    void UpdateServer(QString, int, Server::MsgKind);

private:
    QWidget *myWidget;
    QListWidget *ContentListWidget;
    QLabel *PortLabel;
    QLineEdit *PortLineEdit;
    QPushButton *CreateBtn;
    QTreeWidget *resourceTree;
    QGridLayout *mainLayout;

    int port;
    Server *server;

};

#endif // TCP_SERVER_H
