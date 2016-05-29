#include "myserver.h"

MyServer::MyServer(int port, QWidget *parent)
    : QWidget(parent)
    ,blockSize(0)
{
    listOfconnected = new QVector<QTcpSocket*>();
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost,port))
    {
        QMessageBox::critical(0,"server error","unable to start server"+tcpServer->errorString());
        tcpServer->close();
        return;
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);

    QVBoxLayout* vertLayout=new QVBoxLayout;
    vertLayout->addWidget(new QLabel("<H1>SERVER</H1>"));
    vertLayout->addWidget(textEdit);
    setLayout(vertLayout);
}

bool MyServer::AddConnected(QTcpSocket* newSocket)
{
   if (listOfconnected->contains(newSocket))
       return false;
   listOfconnected->push_back(newSocket);
   return true;
}

void MyServer::sendOthers(QTcpSocket* newSocket,const QByteArray &str)
{
     for (QTcpSocket* element:*listOfconnected)
     {
       if (element==newSocket)
           continue;
       sendToClientByte(element,str);
     }

}
void MyServer::slotNewConnection()
{
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
    if (!AddConnected(clientSocket))
    {
        // will never be here, because of realization of client
        sendToClient(clientSocket, "You have already connected");
        return;
    }
    connect(clientSocket,SIGNAL(disconnected()),clientSocket,SLOT(deleteLater()));
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
    sendToClient(clientSocket, "Server Response: connected!");

}

void MyServer::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    //QDataStream in(clientSocket);
//    in.setVersion(Qd);
//    for (;;)
//    {
//        if (!blockSize)
//        {
//            if (clientSocket->bytesAvailable()<sizeof(quint16))
//            {
//                break;
//            }
//            in>>blockSize;
//        }
//        if (clientSocket->bytesAvailable()<sizeof(quint16))
//        {
//            break;
//        }
        //QTime time;
        //QString str;
        //in

        //QString strMessage=" Client has sent-"+str;
        //qDebug()<<strMessage;
        QByteArray str = clientSocket->readAll();
        //textEdit->append(clientSocket->readAll());
        QRegExp r("disconnected");
        textEdit->append(str);
        sendOthers(clientSocket,str);
        if ( r.indexIn(str)!=-1)
        {
            listOfconnected->remove(listOfconnected->indexOf(clientSocket));
            return;
        }

        //textEdit->append(strMessage);
        sendToClient(clientSocket,"Server response: received");

//        QRegExp q("connected");
//        if ( q.indexIn(str)!=-1)
//        {
//            listOfconnected->push_back(clientSocket);

//        }
    //}
}



void MyServer::sendToClientByte(QTcpSocket *socket, const QByteArray &str)
{
    socket->write(str);
}

void MyServer::sendToClient(QTcpSocket *socket, const QString &str)
{
//    QByteArray arrblock;
//    QDataStream out(&arrblock,QIODevice::WriteOnly);
//    out<<quint16(0)<<QTime::currentTime()<<str;
//    out.device()->seek(0);
//    out<<quint16(arrblock.size()-sizeof(quint16));
//    socket->write(arrblock);
    socket->write(str.toUtf8());
}

MyServer::~MyServer()
{
    delete tcpServer;
    delete textEdit;
}
