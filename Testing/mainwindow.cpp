#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress::LocalHost,8080);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    socket->open(QIODevice::ReadWrite);
    if(socket->isOpen()){
        QMessageBox::information(this, "Status", "Connected to server.");

    }
    else{
        QMessageBox::information(this, "Status", "Unable to connect.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(socket){
        if(socket->isOpen()){
            QString data = ui->lineEdit->text();
            socket->write(data.toStdString().c_str());
        }
    }
    else{
        QMessageBox::information(this, "Status", "Error: " + socket->errorString());
    }
}

void MainWindow::readDataFromSocket(){
    if(socket){
        if(socket->isOpen()){
            QByteArray dataFromServer = socket->readAll();
            QString message = QString::fromStdString(dataFromServer.toStdString());
            ui->textEdit->append(message);
        }
    }
    else{
        QMessageBox::information(this, "Status", "Error: " + socket->errorString());
    }
}
