#include "tcp_server.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("P2P Server");

    ContentListWidget = new QListWidget;

    myWidget = new QWidget;
    PortLabel = new QLabel("端口:");
    PortLineEdit = new QLineEdit;
    CreateBtn = new QPushButton("打开服务器");

    resourceTree = new QTreeWidget();
    resourceTree->setHeaderLabel("在线用户");
    resourceTree->clear();

    mainLayout = new QGridLayout();

    mainLayout->addWidget(ContentListWidget,0,0,1,2);
    mainLayout->addWidget(PortLabel,1,0);
    mainLayout->addWidget(PortLineEdit,1,1);
    mainLayout->addWidget(CreateBtn,2,0,1,2);
    mainLayout->addWidget(resourceTree,0,2,3,1);
    myWidget->setLayout(mainLayout);
    setCentralWidget(myWidget);

    port = 8010;
    PortLineEdit->setText(QString::number(port));
    connect(CreateBtn,SIGNAL(clicked()),this,SLOT(slotCreateServer()));

    server = 0;
}

void MainWindow::slotCreateServer(){
    server = new Server(this, port);
    server->db = QSqlDatabase::addDatabase("QMYSQL");
    server->db.setHostName("localhost");
    server->db.setDatabaseName("ShareFile");
    server->db.setUserName("root");
    server->db.setPassword("root");
    if (!server->db.open()) {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),
        "无法创建数据库连接！ ", QMessageBox::Cancel);
        return;
    }
    connect(server,SIGNAL(UpdateServer(QString,int,Server::MsgKind)),this,SLOT(UpdateServer(QString,int,Server::MsgKind)));
    CreateBtn->setEnabled(false);
}

void MainWindow::UpdateServer(QString msg, int length, Server::MsgKind flag){
    switch (flag) {
    case Server::UpdateMsg:
    {
        ContentListWidget->addItem(msg.left(length));
        break;
    }
    case Server::UPDATEMETA:
    {
        ContentListWidget->addItem(msg + ": update meta");
        break;
    }
    case Server::UpdateName:
    {
        QTreeWidgetItem *item_name = new QTreeWidgetItem(resourceTree);
        item_name->setText(0,msg);
        break;
    }
    case Server::RemoveName:
    {
        QPoint p(length,0);
        resourceTree->removeItemWidget(resourceTree->itemAt(p),0);
        delete resourceTree->itemAt(p);
        break;
    }
    }
}

MainWindow::~MainWindow()
{
    delete PortLabel;
    delete PortLineEdit;
    delete CreateBtn;
    delete ContentListWidget;
    resourceTree->clear();
    delete resourceTree;
    delete mainLayout;
    delete myWidget;
    if(server != 0){
        server->db.close();
        delete server;
    }
}
