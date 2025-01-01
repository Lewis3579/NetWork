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

    QByteArray messageFromClient = socket->readAll();
    QString messagePostProcessing = QString::fromStdString(messageFromClient.toStdString());
    QString message = "Client " + QString::number(socket->socketDescriptor()) + ": " + messagePostProcessing;

    ui->textEdit->append(message);


    if(messagePostProcessing.contains("1000")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        fileName = responseList.at(responseList.count()-2);

        fileSize = responseList.at(responseList.count()-1).toInt();
        qDebug() << fileName;
        response = "0000|UPLOAD_SUCCESS";
        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
    }

    if(messagePostProcessing.contains("1005")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        QString username = responseList.at(responseList.count()-2);
        QString password = responseList.at(responseList.count()-1);

        qDebug() << username;
        qDebug() << password;

        QSqlQuery query("SELECT * FROM users");
        int fieldUserName = query.record().indexOf("username");
        int fieldPassword = query.record().indexOf("password");
        bool existedUsername = false;
        while (query.next()) {
            QString usernameFromDB = query.value(fieldUserName).toString();
            QString passwordFromDB = query.value(fieldPassword).toString();

            if(QString::compare(username,usernameFromDB)==0){
                existedUsername = true;
                if(QString::compare(password,passwordFromDB)==0){
                    response = "0010|SIGNIN_SUCCESS";
                    foreach (QTcpSocket* socket, clientList) {
                        socket->write(response.toStdString().c_str());
                    }
                }
                else{
                    response = "0011|WRONG_PASSWORD";
                    foreach (QTcpSocket* socket, clientList) {
                        socket->write(response.toStdString().c_str());
                    }
                }
            }
        }

        if(existedUsername == false){
            response = "0012|NO_USERNAME_IN_DATABASE";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1006")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        QString username = responseList.at(responseList.count()-2);
        QString password = responseList.at(responseList.count()-1);

        qDebug() << username;
        qDebug() << password;

        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password) "
                      "VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.exec();
        if(query.lastError().isValid()){
            ui->textEdit->setText(query.lastError().text());
            response = "0013|EXISTED_USERNAME";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
        else{
            response = "0014|SIGN_UP_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1003")){
        ui->textEdit->append("request");


        response = "0006|GET_ALL_FILE_SUCCESS|";
        QSqlQuery query("SELECT * FROM files");
        int fieldFileName = query.record().indexOf("file_name");
        while (query.next()) {
            QString filenameFromDB = query.value(fieldFileName).toString();
            response=response+filenameFromDB+"|";
        }

        response.removeLast();
        qDebug() << response;

        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
    }
}

void MainWindow::readSpecialFromSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray messageFromServer = socket->readAll();
    //qDebug() << messageFromServer;
    QString filePath = "D:\\dataRoot\\" + fileName;
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Cant open selected file.";
    }
    file.write(messageFromServer);

    QSqlQuery query;
    query.prepare("INSERT INTO files (filename) "
                  "VALUES (:filename)");
    query.bindValue(":filename", fileName);
    query.exec();


    fileSize = fileSize - messageFromServer.size();
    ui->textEdit->append(QString::number(fileSize));
    if(fileSize>0){
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialLargeFromSocket()));

    }
    else{
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    }
}

void MainWindow::readSpecialLargeFromSocket()
{

    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray messageFromServer = socket->readAll();
    //qDebug() << messageFromServer;
    QString filePath = "D:\\dataRoot\\" + fileName;
    QFile file(filePath);
    if(!file.open(QIODevice::Append)){
        qDebug() << "Cant open selected file.";
    }
    file.write(messageFromServer);

    fileSize = fileSize - messageFromServer.size();
    ui->textEdit->append(QString::number(fileSize));
    if(fileSize <= 0){
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialLargeFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    }
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

