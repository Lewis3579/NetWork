#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("FileTransfer");
    db.setUserName("postgres");
    db.setPassword("Khanh2003.");
    bool ok = db.open();
    if (ok){
        qDebug() << "Open database successfully";
    }
    else{
        qDebug() << "Fail to open database";
    }

    server = new QTcpServer();

    if(server->listen(QHostAddress::AnyIPv4, 1234)){
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
    QString messagePostProcessing = QString::fromStdString(messageFromServer.toStdString());
    QString message = "Client " + QString::number(socket->socketDescriptor()) + ": " + messagePostProcessing;

    ui->textEdit->append(message);

    if(QString::compare(messagePostProcessing,"1000")==0){
        ui->textEdit->append("command");
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
    }
}

void MainWindow::readSpecialFromSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray messageFromServer = socket->readAll();
    QString messagePostProcessing = QString::fromStdString(messageFromServer.toStdString());
    QString message = "Client " + QString::number(socket->socketDescriptor()) + " special: " + messagePostProcessing;
    ui->textEdit->append(message);

    QFile file("D:\\dataRoot\\dataContainer.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug() << "Cant open selected file.";
    }
    QTextStream out(&file);
    out << messagePostProcessing;

    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
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

