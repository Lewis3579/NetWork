#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QPushButton>

namespace Ui {
class SignIn;
}

class SignIn : public QDialog
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent);
    explicit SignIn(QWidget *parent, QTcpSocket* socket);
    ~SignIn();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::SignIn *ui;
    QTcpSocket* socketLogin;
    QString request;

public:
    void notifySignInError(QString notification);
};

#endif // SIGNIN_H
