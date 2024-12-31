#include "signin.h"
#include "ui_signin.h"

SignIn::SignIn(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignIn)
{
    ui->setupUi(this);

}

SignIn::SignIn(QWidget *parent, QTcpSocket* socket)    : QDialog(parent)
    , ui(new Ui::SignIn)
{
    ui->setupUi(this);
    socketLogin = socket;

    connect(ui->pushButton,SIGNAL(clicked()),parent,SLOT(transitionToSignUp()));
}
SignIn::~SignIn()
{
    delete ui;
}

void SignIn::on_pushButton_2_clicked()
{
    if(socketLogin){
        if(socketLogin->isOpen()){
            request = "1005|SIGN_IN|" + ui->lineEdit->text() + "|" + ui->lineEdit_2->text();
            socketLogin->write(request.toStdString().c_str());
        }
    }
    else{
        QMessageBox::information(this, "Status", "Error: " + socketLogin->errorString());
    }

}

void SignIn::notifySignInError(QString notification)
{

    ui->textEdit->setText(notification);
}

