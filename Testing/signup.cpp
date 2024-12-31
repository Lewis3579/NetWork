#include "signup.h"
#include "ui_signup.h"

SignUp::SignUp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);
}

SignUp::SignUp(QWidget *parent, QTcpSocket *socket)    : QDialog(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);
    socketSignUp = socket;

    connect(ui->pushButton_2,SIGNAL(clicked()),parent,SLOT(transitionToSignIn()));
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::notifyDuplicateUsername(QString notification)
{

    ui->textEdit->setText(notification);
}

void SignUp::on_pushButton_clicked()
{
    if(socketSignUp){
        if(socketSignUp->isOpen()){
            request = "1006|SIGN_UP|" + ui->lineEdit->text() + "|" + ui->lineEdit_2->text();
            socketSignUp->write(request.toStdString().c_str());
        }
    }
    else{
        QMessageBox::information(this, "Status", "Error: " + socketSignUp->errorString());
    }
}

