#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QGridLayout>
#include <QString>
#include <QTableWidget>
#include <QList>
#include <QTimer>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "p2p_download.h"
#include "p2p_server.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum MsgKind{
        NAMEMSG = 0,
        METAMSG = 1,
        DISCONNECT = 2,
        SHAREMSG = 3,
        DELETEMSG = 4,
        SEARCHMSG = 5,
        SIGNMSG = 6,
        OTHERMSG = 7
    };

private:
    QWidget *myWidget;

    QListWidget *ContentListWidget;

    QLabel *UserNameLabel;
    QLineEdit *UserNameLineEdit;

    QLabel *UserPasswdLabel;
    QLineEdit *UserPasswdLineEdit;

    QLabel *ServerIPLabel;
    QLineEdit *ServerIPLineEdit;

    QLabel *PortLabel;
    QLineEdit *PortLineEdit;

    QPushButton *EnterBtn;
    QPushButton *SignBtn;

    QTableWidget *ResultTableWidget;
    QLineEdit *SearchLineEdit;
    QPushButton *SearchBtn;
    QProgressBar *ProgressBar;
    QPushButton *DownloadBtn;
    QTimer *timer;

    QTableWidget *ShareTableWidget;
    QPushButton *FlushBtn;
    QPushButton *DisableBtn;
    QPushButton *ShareBtn;

    QLineEdit *DirLineEdit;
    QPushButton *SelectDirBtn;

    QGridLayout *layout1;
    QGridLayout *layout2;
    QGridLayout *layout3;
    QGridLayout *mainLayout;

    bool status;
    int port;
    QHostAddress *ServerIP;
    QString UserName;
    QTcpSocket *tcpSocket;
    P2P_Server *p2pserver;
    QList<P2P_Download*> tasks;

    QString ClientDir;

    void FormatMsg(QString &msg, MsgKind kind);
    void UpdateShareTableWidget(QString &msg);
    void UpdateResultTableWidget(QString &msg);
public slots:
    void slotEnter();
    void slotSendMeta();
    void slotConnected();
    void slotDisconnected();
    void DataReceived();
    void slotFlushShare();
    void slotDeleteShare();
    void slotSearch();
    void slotSelectDir();
    void slotDownload();

    void slotSignUp();
    void SignConnected();
    void SignDisconnected();
    void SignDataReceived();

    void timeout();

};

#endif // TCP_CLIENT_H
