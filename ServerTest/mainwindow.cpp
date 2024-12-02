#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer();

    if(server->listen(QHostAddress::Any, 8080)){
        connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()));
        QMessageBox::information(this, "Status", "Server started.");
    }
    else{
        QMessageBox::information(this, "Status", "Unable to start server.");
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnection()
{
    while(server->hasPendingConnections()){
        addNewClient(server->nextPendingConnection());
    }
}

void MainWindow::readDataFromSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray messageFromServer = socket->readAll();
    QString message = "Client " + QString::number(socket->socketDescriptor()) + ": " + QString::fromStdString(messageFromServer.toStdString());

    ui->textEdit->append(message);
}

void MainWindow::addNewClient(QTcpSocket* socket)
{
    clientList.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    ui->comboBox->addItem(QString::number(socket->socketDescriptor()));
    QString Client = "Client " + QString::number(socket->socketDescriptor()) + ": Connected with server.";
    ui->textEdit->append(Client);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString message = ui->lineEdit->text();
    QString receiver = ui->comboBox->currentText();
    if(ui->comboBox_2->currentText() == "All"){
        foreach (QTcpSocket* socket, clientList) {

            socket->write(message.toStdString().c_str());


        }
    }
    else{
        foreach (QTcpSocket* socket, clientList) {
            if(socket->socketDescriptor()== receiver.toLongLong()){
                socket->write(message.toStdString().c_str());
            }

        }
    }
}

