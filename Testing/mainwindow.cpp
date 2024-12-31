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
        QStringList filenameList = response.split(u'|');
        for(int i = 2; i < filenameList.count();i++){
            QPushButton* pushButton = new QPushButton();
            pushButton->setText(filenameList[i]);
            FileToDownload* fileButton = new FileToDownload();
            fileButton->setFileButton(pushButton);
            fileList.append(fileButton);
        }

        for(int i = 0; i<fileList.count();i++){
            scrollLayout->addWidget(fileList[i]->getFileButton());
            connect(fileList[i]->getFileButton(), SIGNAL(clicked()), this, SLOT(downloadFromServer()));
        }
        scrollWidget->setLayout(scrollLayout);
        ui->scrollArea->setWidget(scrollWidget);
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

    QStringList pathList = filePath.split(u'/');
    QString fileName = pathList.at(pathList.count()-1);
    request = request + fileName + "|" + QString::number( buffer.size());

    qDebug() << request;
    sendData(request);
}

void MainWindow::sendData(QString bufferString)
{

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
    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);

    fileList.clear();
    scrollLayout->setAlignment(Qt::AlignTop);

    request = "1003|LIST_FILE";
    sendData(request);
/*
    for(int i = 0; i < 20; i++){
        QPushButton* pushButton = new QPushButton();
        pushButton->setText(QString::number(i));

        FileToDownload* fileButton = new FileToDownload();
        fileButton->setFileButton(pushButton);
        fileList.append(fileButton);
    }

    for(int i = 0; i<fileList.count();i++){
        scrollLayout->addWidget(fileList[i]->getFileButton());
    }
    scrollWidget->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollWidget);
*/

}


void MainWindow::on_lineEdit_2_textChanged(const QString &textInLine)
{
    for(int i = 0; i<fileList.count();i++){
        scrollLayout->removeWidget(fileList[i]->getFileButton());
        fileList[i]->getFileButton()->hide();
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

void MainWindow::downloadFromServer()
{
    qDebug() << "Clicked";
    request = "1001|DOWNLOAD";
    sendData(request);
}
