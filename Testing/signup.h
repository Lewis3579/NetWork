#ifndef SIGNUP_H
#define SIGNUP_H

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

namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    explicit SignUp(QWidget *parent, QTcpSocket* socket);
    ~SignUp();

private:
    Ui::SignUp *ui;
    QTcpSocket* socketSignUp;
    QString request;

public:
    void notifyDuplicateUsername(QString notification);
private slots:
    void on_pushButton_clicked();
};

#endif // SIGNUP_H
