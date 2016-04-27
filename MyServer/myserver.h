#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QHostAddress>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QString>
#include <QVector>
#include <QRect>
#include <QWidget>
#include <QSslSocket>

class MyServer : public QWidget
{
    Q_OBJECT

public:
    MyServer(int port,QWidget *parent = 0);
    ~MyServer();
private:
    QTcpServer* tcpServer;
    QTextEdit* textEdit;
    quint16 blockSize;
    void sendToClient(QTcpSocket* socket,const QString& str);
    QVector<QTcpSocket*> *listOfconnected;
    bool AddConnected(QTcpSocket* newSocket);
    void sendOthers(QTcpSocket* newSocket,const QByteArray &str);
    void sendToClientByte(QTcpSocket *socket, const QByteArray &str);
public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

#endif // MYSERVER_H
