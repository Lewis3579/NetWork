#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress::LocalHost,1234);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    socket->open(QIODevice::ReadWrite);
    if(socket->isOpen()){
        QMessageBox::information(this, "Status", "Connected to server.");

    }
    else{
        QMessageBox::information(this, "Status", "Unable to connect.");
    }

    signInWindow = new SignIn(this, socket);
    signInWindow->show();

    signUpWindow = new SignUp(this, socket);
    //scrollLayout->setAlignment(Qt::AlignTop);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString data = ui->lineEdit->text();
    sendData(data);
}

void MainWindow::readDataFromSocket(){

    QString response = receiveCommand();
    ui->textEdit->append(response);
    if(response.contains("0000")){
        ui->textEdit->append("response");
        //qDebug() << buffer;
        socket->write(buffer);
    }

    if(response.contains("0008")){
        ui->textEdit->append("response");
        ui->textEdit->append("response");
    }
    if(response.contains("0010")){
        ui->textEdit->append("response");
        signInWindow->hide();
        this->show();
    }

    if(response.contains("0011")){
        ui->textEdit->append("response");
        signInWindow->notifySignInError(response);
    }

    if(response.contains("0012")){
        ui->textEdit->append("response");
        signInWindow->notifySignInError(response);
    }

    if(response.contains("0014")){
        ui->textEdit->append("response");
        signUpWindow->hide();
        signInWindow->show();
    }

    if(response.contains("0013")){
        ui->textEdit->append("response");
        signUpWindow->notifyDuplicateUsername(response);
    }

    if(response.contains("0006")){
        ui->textEdit->append("response");
        QStringList filePathList = response.split(u'|');
        for(int i = 2; i < filePathList.count();i++){
            FileToDownload* fileButton = new FileToDownload();
            QStringList fileInfoList = filePathList[i].split(u'#');
            fileButton->setFileID(fileInfoList.at(0).toInt());
            fileButton->setFolderID(fileInfoList.at(1).toInt());
            fileButton->setFileSIze(fileInfoList.at(2).toInt());

            fileButton->setFilePath(fileInfoList.at(fileInfoList.count()-1));
            QPushButton* pushButton = new QPushButton();
            pushButton->setIcon(QIcon(":/file_icon.png"));

            QStringList fileNameList = filePathList[i].split(u'/');
            pushButton->setText(fileNameList.at(fileNameList.count()-1));
            fileButton->setFileName(fileNameList.at(fileNameList.count()-1));

            fileButton->setFileButton(pushButton);
            fileList.append(fileButton);
        }

        for(int i = 0; i<fileList.count();i++){
            if(fileList[i]->getFolderID()==currentFolder) scrollLayout->addWidget(fileList[i]->getFileButton());
            connect(fileList[i]->getFileButton(), SIGNAL(clicked()), fileList[i], SLOT(downloadFileFromServer()));
        }
        scrollWidget->setLayout(scrollLayout);
        ui->scrollArea->setWidget(scrollWidget);

        request = "1010|LIST_FOLDER";
        sendData(request);
    }

    if(response.contains("0015")){
        ui->textEdit->append("response");
    }

    if(response.contains("0023")){
        ui->textEdit->append("response");
        QStringList folderPathList = response.split(u'|');
        for(int i = 2; i < folderPathList.count();i++){

            FolderToDownload* folderButton = new FolderToDownload();
            QStringList fileInfoList = folderPathList[i].split(u'#');
            folderButton->setFolderID(fileInfoList.at(0).toInt());
            folderButton->setParentID((fileInfoList.at(1).toInt()));


            folderButton->setFolderPath(fileInfoList.at(fileInfoList.count()-1));
            QPushButton* pushButton = new QPushButton();
            pushButton->setIcon(QIcon(":/folder_icon.png"));

            QStringList fileNameList = folderPathList[i].split(u'/');
            pushButton->setText(fileNameList.at(fileNameList.count()-1));
            folderButton->setFolderName(fileNameList.at(fileNameList.count()-1));
            folderButton->setFolderButton(pushButton);

            folderList.append(folderButton);
        }

        for(int i = 0; i<folderList.count();i++){
            if(folderList[i]->getParentID()==currentFolder&&folderList[i]->getFolderID()!=0) scrollLayout->addWidget(folderList[i]->getFolderButton());
            connect(folderList[i]->getFolderButton(), SIGNAL(clicked()), folderList[i], SLOT(downloadFolderFromServer()));
        }

        scrollWidget->setLayout(scrollLayout);
        ui->scrollArea->setWidget(scrollWidget);
    }
}

void MainWindow::downloadDataFromSocket()
{
    QByteArray dataDownload = socket->readAll();
    QString locationPath = "D:\\" + fileDownloadName;
    QFile file(locationPath);

    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Cant open selected file.";
    }

    file.write(dataDownload);

    this->ui->textEdit->append(QString::number(fileDownloadSize));
    fileDownloadSize = fileDownloadSize - dataDownload.size();

    if(fileDownloadSize>0){
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(downloadDataFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(downloadLargeDataFromSocket()));
    }
    else{
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(downloadDataFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    }
}

void MainWindow::downloadLargeDataFromSocket()
{
    QByteArray dataDownload = socket->readAll();
    QString locationPath = "D:\\" + fileDownloadName;
    QFile file(locationPath);

    if(!file.open(QIODevice::Append)){
        qDebug() << "Cant open selected file.";
    }

    file.write(dataDownload);

    this->ui->textEdit->append(QString::number(fileDownloadSize));
    fileDownloadSize = fileDownloadSize - dataDownload.size();

    if(fileDownloadSize <= 0){
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(downloadLargeDataFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    }
}

void MainWindow::readFileDownloaded()
{
    QString response = receiveCommand();
    ui->textEdit->append(response);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open files"), "D:\\");
    ui->label->setText(filePath);
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Cant open selected file.";
    }


    buffer = file.readAll();
    request = "1000|UPLOAD|";

    if(ui->checkBox->isChecked()){
        request = request + "Private|";
    }
    else{
        request = request + "Public|";
    }

    QStringList pathList = filePath.split(u'/');
    QString fileName = pathList.at(pathList.count()-1);
    request = request + fileName + "|" + QString::number( buffer.size());
}

void MainWindow::sendData(QString bufferString)
{

    QFile log("D:\\log.txt");
    if(!log.open(QIODevice::Append|QIODevice::Text)){
        qDebug() << "Can not open log";
    }

    QTextStream out(&log);
    out << QTime::currentTime().toString() << "\n";
    out << bufferString << "\n";

    if(socket){
        if(socket->isOpen()){
            socket->write(bufferString.toStdString().c_str());
        }
    }
    else{
        QMessageBox::information(this, "Status", "Error: " + socket->errorString());
    }
}

QString MainWindow::receiveCommand()
{
    if(socket){
        if(socket->isOpen()){
            QByteArray dataFromServer = socket->readAll();
            QString message = QString::fromStdString(dataFromServer.toStdString());
            return message;
        }
    }
    else{
        QMessageBox::information(this, "Status", "Error: " + socket->errorString());
    }
}

void MainWindow::transitionToSignUp()
{
    signInWindow->hide();
    signUpWindow->show();
}

void MainWindow::transitionToSignIn()
{
    signUpWindow->hide();
    signInWindow->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    signInWindow->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    scrollWidget = new QWidget();
    scrollLayout = new QVBoxLayout();

    for(int i = 0; i<fileList.count();i++){
        scrollLayout->removeWidget(fileList[i]->getFileButton());
        fileList[i]->getFileButton()->hide();
    }

    for(int i = 0; i<folderList.count();i++){
        scrollLayout->removeWidget(folderList[i]->getFolderButton());
        folderList[i]->getFolderButton()->hide();
    }

    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);

    fileList.clear();
    folderList.clear();
    scrollLayout->setAlignment(Qt::AlignTop);

    request = "1003|LIST_FILE";
    sendData(request);

    currentFolder = 0;

}


void MainWindow::on_lineEdit_2_textChanged(const QString &textInLine)
{
    for(int i = 0; i<fileList.count();i++){
        scrollLayout->removeWidget(fileList[i]->getFileButton());
        fileList[i]->getFileButton()->hide();
    }

    for(int i = 0; i<folderList.count();i++){
        scrollLayout->removeWidget(folderList[i]->getFolderButton());
        folderList[i]->getFolderButton()->hide();
    }
    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);


    for(int i = 0; i<fileList.count();i++){
        if(fileList[i]->getFileName().contains(textInLine,Qt::CaseInsensitive)){
            scrollLayout->addWidget(fileList[i]->getFileButton());
            fileList[i]->getFileButton()->show();
        }
    }
    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);
}



void MainWindow::on_pushButton_5_clicked()
{
    qDebug() << request;
    sendData(request);
}


void MainWindow::on_pushButton_6_clicked()
{
    if(ui->lineEdit_3->text().isEmpty()){
        qDebug() << "You must enter folder name";
    }
    else{
        request = "1007|CREATE_FOLDER|";
        if(ui->checkBox_2->isChecked()){
            request = request + "Private|";
        }
        else{
            request = request + "Public|";
        }
        request = request + ui->lineEdit_3->text();
        sendData(request);
    }
}


void MainWindow::on_pushButton_7_clicked()
{
    QFile file("D:\\fileINFO.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Cannot read file info";
    }

    QString info = file.readAll();
    QStringList infoList = info.split(u'|');
    fileDownloadPath = infoList.at(0);
    fileDownloadName = infoList.at(1);
    fileDownloadID = infoList.at(2).toInt();
    fileDownloadFolder = infoList.at(3).toInt();
    fileDownloadSize = infoList.at(4).toInt();


    request = "1001|DOWNLOAD|" + QString::number( fileDownloadID);
    sendData(request);

    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(downloadDataFromSocket()));
}


void MainWindow::on_pushButton_8_clicked()
{

    QFile file("D:\\fileINFO.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Cannot read file info";
    }

    QString info = file.readAll();
    QStringList infoList = info.split(u'|');
    fileDownloadPath = infoList.at(0);
    fileDownloadName = infoList.at(1);
    fileDownloadID = infoList.at(2).toInt();
    fileDownloadFolder = infoList.at(3).toInt();
    fileDownloadSize = infoList.at(4).toInt();

    request = "1004|DELETE_FILE|" + QString::number( fileDownloadID);
    sendData(request);
}


void MainWindow::on_pushButton_9_clicked()
{
    QFile folder("D:\\folderINFO.txt");
    if(!folder.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Cannot read folder info";
    }

    QString info = folder.readAll();
    QStringList infoList = info.split(u'|');
    folderDownloadPath = infoList.at(0);
    folderDownloadName = infoList.at(1);
    folderDownloadID = infoList.at(2).toInt();
    folderDownloadParent = infoList.at(3).toInt();

    for(int i = 0; i<fileList.count();i++){
        scrollLayout->removeWidget(fileList[i]->getFileButton());
        fileList[i]->getFileButton()->hide();
    }

    for(int i = 0; i<folderList.count();i++){
        scrollLayout->removeWidget(folderList[i]->getFolderButton());
        folderList[i]->getFolderButton()->hide();
    }
    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);

    for(int i = 0; i<fileList.count();i++){
        if(fileList[i]->getFolderID()==folderDownloadID) {
            scrollLayout->addWidget(fileList[i]->getFileButton());
            fileList[i]->getFileButton()->show();
        }
    }

    for(int i = 0; i<folderList.count();i++){
        if(folderList[i]->getParentID()==folderDownloadID&&folderList[i]->getFolderID()!=0) {
            scrollLayout->addWidget(folderList[i]->getFolderButton());
            folderList[i]->getFolderButton()->show();
        }
    }

    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);

    request = "1011|MOVE_TO|" + QString::number( folderDownloadID);
    sendData(request);
}


void MainWindow::on_pushButton_10_clicked()
{

    QFile folder("D:\\folderINFO.txt");
    if(!folder.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Cannot read folder info";
    }

    QString info = folder.readAll();
    QStringList infoList = info.split(u'|');
    folderDownloadPath = infoList.at(0);
    folderDownloadName = infoList.at(1);
    folderDownloadID = infoList.at(2).toInt();
    folderDownloadParent = infoList.at(3).toInt();

    request = "1008|DELETE_FOLDER|" + QString::number( folderDownloadID);
    sendData(request);
}


void MainWindow::on_pushButton_11_clicked()
{
    QFile file("D:\\fileINFO.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Cannot read file info";
    }

    QString info = file.readAll();
    QStringList infoList = info.split(u'|');
    fileDownloadPath = infoList.at(0);
    fileDownloadName = infoList.at(1);
    fileDownloadID = infoList.at(2).toInt();
    fileDownloadFolder = infoList.at(3).toInt();
    fileDownloadSize = infoList.at(4).toInt();

    request = "1012|RENAME_FILE|" + this->ui->lineEdit_4->text() + "|" + QString::number( fileDownloadID);
    sendData(request);
}

