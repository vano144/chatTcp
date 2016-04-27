#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>

namespace Ui {
class dialog;
}

class dialog : public QDialog
{
    Q_OBJECT

public:
    explicit dialog(const int port,QWidget *parent = 0);
    ~dialog();
    QString getUserName();
    void showMessage(const QString &message);

private:
//    Ui::dialog *ui;
    void setUserName(const QString &name);
    void setDesign();
    QTcpSocket *socket; //сокет
    quint16 blockSize;//текущий размер блока данных
    QString userName;//имя клиента
    QTextEdit *textEdit;
    QPushButton *sendButton;
    QPushButton *connectButton;
    QPushButton *deleteButton;
    QTextEdit *fieldForName;
    QTextEdit *fieldForMessage;
    int localPort;
private slots:
    void connectToServer();
    void sendMessage();
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();
    void sendlastMessage();
    //void onSokDisplayError(QAbstractSocket::SocketError socketError);

};

#endif // DIALOG_H
