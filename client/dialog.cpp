#include "dialog.h"
#include "ui_dialog.h"

//--------------------- ui part
void dialog::setDesign()
{
    this->setGeometry(0,0,320,420);
    sendButton = new QPushButton("Send message");
    connectButton = new QPushButton("Connect to server");
    textEdit=new QTextEdit;
    textEdit->setReadOnly(true);
    QVBoxLayout* verticalLayout=new QVBoxLayout;
    verticalLayout->setMargin(5);
    verticalLayout->setSpacing(15);
    verticalLayout->addWidget(textEdit);
    QHBoxLayout* verticalLayout1=new QHBoxLayout;
    verticalLayout1->setMargin(5);
    verticalLayout1->setSpacing(15);
    fieldForName = new QTextEdit();
    verticalLayout1->addWidget(fieldForName);

    fieldForMessage = new QTextEdit();
    verticalLayout1->addWidget(fieldForMessage);


    QHBoxLayout* horizontalLayout=new QHBoxLayout;
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(15);
    horizontalLayout->addWidget(sendButton);
    horizontalLayout->addWidget(connectButton);
    deleteButton = new QPushButton("Dissconnect");
    horizontalLayout->addWidget(deleteButton);
    verticalLayout->addLayout(verticalLayout1);
    verticalLayout->addLayout(horizontalLayout);

    this->setLayout(verticalLayout);
    this->show();
}

void dialog::showMessage(const QString &message)
{
    textEdit->append(message);
}

void dialog::socketConnected()
{
    showMessage("the connection is established");
}

void dialog::socketDisconnected()
{
    showMessage("the connection is cut off");
}
//---------------------

void dialog::setUserName(const QString &name)
{
    userName=name;
}
QString dialog::getUserName()
{
    return userName;
}



dialog::dialog(const int port,QWidget *parent) : QDialog(parent)
//    ui(new Ui::dialog)
{
    localPort=port;
    setUserName("");
    setDesign();

    socket = new QTcpSocket(this);
    //ui->setupUi(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(connectToServer()));
    connect(sendButton,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));
    connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(sendlastMessage()));
    sendButton->setDisabled(true);
    deleteButton->setDisabled(true);

}
void dialog::sendlastMessage()
{
     socket->write((getUserName()+" disconnected").toUtf8());
     //socket->close();
     socket->disconnect();
     socket->close();
     textEdit->append("You are disconnected");
     connectButton->setDisabled(false);
     sendButton->setDisabled(true);
     deleteButton->setDisabled(true);
}

void dialog::connectToServer()
{
    if (getUserName()=="")
    {
        setUserName(fieldForName->toPlainText());
        fieldForName->setReadOnly(true);
    }
    if (getUserName()=="") return;
    socket->connectToHost(QHostAddress::LocalHost,localPort);
    socket->write((getUserName()+"is connected").toUtf8());
    connectButton->setDisabled(true);
    sendButton->setDisabled(false);
    deleteButton->setDisabled(false);
}



void dialog::socketReadyRead()
{
    textEdit->append(socket->readAll());
}

void dialog::sendMessage()
{
    QString message;
    message = fieldForMessage->toPlainText();
    if (message == "")
        return;
    message = getUserName() + " writes: " + message;
    showMessage(message);
    socket->write(message.toLatin1());
    fieldForMessage->clear();
    qDebug()<<message.toUtf8();

}


dialog::~dialog()
{
    socket->disconnect();
    delete socket;

    delete textEdit;
    delete sendButton;
    delete connectButton;
    delete deleteButton;
    delete fieldForName;
    delete fieldForMessage;
}
