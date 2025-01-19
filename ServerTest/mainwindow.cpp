#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    rootDir.setPath("D:\\dataRoot");
    tempDir = rootDir;
    folderCurrentID = 0;
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
        fileState = responseList.at(responseList.count()-3);
        qDebug() << fileName;

        QString filePath = tempDir.absolutePath() + "/" + fileName;

        QSqlQuery compareQuery;
        compareQuery.prepare("SELECT * FROM files WHERE file_name = :fileName");
        compareQuery.bindValue(":fileName", fileName);
        compareQuery.exec();

        int fieldFileState = compareQuery.record().indexOf("state");
        int fieldFileOwner = compareQuery.record().indexOf("owner_id");
        int fieldFileFolder = compareQuery.record().indexOf("folder_id");

        bool nameChecked = false;
        bool canUpload = true;
        while(compareQuery.next()){
            if(compareQuery.value(fieldFileFolder).toInt()==folderCurrentID){
                nameChecked = true;
                if(compareQuery.value(fieldFileState).toString()=="Private"){
                    if(compareQuery.value(fieldFileOwner).toInt()!= userID){
                        canUpload = false;
                        response = "0002|FILE_EXIST_IN_SAME_FOLDER";
                        foreach (QTcpSocket* socket, clientList) {
                            socket->write(response.toStdString().c_str());
                        }
                    }
                }
            }
        }

        if(canUpload==true){
            response = "0000|UPLOAD_STARTING";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
            disconnect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
            connect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
            if(nameChecked==false){
                QSqlQuery query;
                query.prepare("INSERT INTO files (file_name,file_path,state,folder_id,owner_id, file_size) "
                              "VALUES (:fileName,:filePath,:state,:folderID,:ownerID, :fileSize)");
                query.bindValue(":fileName", fileName);
                query.bindValue(":filePath", filePath);
                query.bindValue(":state", fileState);
                query.bindValue(":folderID", folderCurrentID);
                query.bindValue(":ownerID", userID);
                query.bindValue(":fileSize", fileSize);
                query.exec();
            }
        }
    }

    if(messagePostProcessing.contains("1001")){
        ui->textEdit->append("request");

        QFile fileDownLoad(fileDownloadPath);
        if(!fileDownLoad.open(QIODevice::ReadOnly)){
            qDebug() << "Cant open selected file.";
        }
        QByteArray buffer =fileDownLoad.readAll();
        foreach (QTcpSocket* socket, clientList) {
            socket->write(buffer);
        }
    }

    if(messagePostProcessing.contains("1013")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        fileDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM files WHERE id = :fileID");
        query.bindValue(":fileID", fileDownloadID);
        query.exec();
        int fieldFileName = query.record().indexOf("file_name");
        int fieldFilePath = query.record().indexOf("file_path");
        int fieldFileState = query.record().indexOf("state");
        int fieldFileOwner = query.record().indexOf("owner_id");

        while(query.next()){
            fileDownloadName = query.value(fieldFileName).toString();
            fileDownloadPath = query.value(fieldFilePath).toString();
            fileDownloadState = query.value(fieldFileState).toString();
            fileDownloadOwner = query.value(fieldFileOwner).toInt();
        }

        QFile fileDownLoad(fileDownloadPath);

        if (fileDownloadState == "Private"){
            if (fileDownloadOwner != userID){
                response = "0026|CAN_NOT_ACCESS";

                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
            else{

                if(!fileDownLoad.open(QIODevice::ReadOnly)){
                    qDebug() << "Cant open selected file.";
                    response = "0007|FILE_NOT_FOUND";
                }
                else{
                    response = "0003|DOWNLOAD_STARTING";
                }

                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }
        else{
            if(!fileDownLoad.open(QIODevice::ReadOnly)){
                qDebug() << "Cant open selected file.";
                response = "0007|FILE_NOT_FOUND";
            }
            else{
                response = "0003|DOWNLOAD_STARTING";
            }
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }

        /*
        QFile fileDownLoad(fileDownloadPath);
        if(fileDownLoad.open(QIODevice::ReadOnly)){
            qDebug() << "Cant open selected file.";
        }
        QByteArray buffer =fileDownLoad.readAll();
        foreach (QTcpSocket* socket, clientList) {
            socket->write(buffer);
        }
        */
    }

    if(messagePostProcessing.contains("1004")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        fileDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM files WHERE id = :fileID");
        query.bindValue(":fileID", fileDownloadID);
        query.exec();

        int fieldFileName = query.record().indexOf("file_name");
        int fieldFilePath = query.record().indexOf("file_path");
        int fieldFileState = query.record().indexOf("state");
        int fieldFileOwner = query.record().indexOf("owner_id");

        while(query.next()){
            fileDownloadName = query.value(fieldFileName).toString();
            fileDownloadPath = query.value(fieldFilePath).toString();
            fileDownloadState = query.value(fieldFileState).toString();
            fileDownloadOwner = query.value(fieldFileOwner).toInt();
        }

        if (fileDownloadState == "Private"){
            if (fileDownloadOwner != userID){
                response = "0026|CAN_NOT_ACCESS";

                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
            else{
                QFile fileDelete(fileDownloadPath);
                if(!fileDelete.open(QIODevice::ReadOnly)){
                    qDebug() << "Cant open selected file.";
                }

                fileDelete.remove();

                QSqlQuery queryDelete;
                queryDelete.prepare("DELETE FROM files WHERE id = :fileID");
                queryDelete.bindValue(":fileID", fileDownloadID);
                queryDelete.exec();

                response = "0008|DELETE_FILE_SUCCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }
        else{
            QFile fileDelete(fileDownloadPath);
            if(!fileDelete.open(QIODevice::ReadOnly)){
                qDebug() << "Cant open selected file.";
            }

            fileDelete.remove();

            QSqlQuery queryDelete;
            queryDelete.prepare("DELETE FROM files WHERE id = :fileID");
            queryDelete.bindValue(":fileID", fileDownloadID);
            queryDelete.exec();

            response = "0008|DELETE_FILE_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1008")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        folderDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM folders WHERE id = :folderID");
        query.bindValue(":folderID", folderDownloadID);
        query.exec();

        int fieldFolderName = query.record().indexOf("folder_name");
        int fieldFolderPath = query.record().indexOf("folder_path");

        while(query.next()){
            folderDownloadName = query.value(fieldFolderName).toString();
            folderDownloadPath = query.value(fieldFolderPath).toString();
        }

        QDir folderDelete(folderDownloadPath);


        folderDelete.removeRecursively();

        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM folders WHERE id = :folderID");
        queryDelete.bindValue(":folderID", folderDownloadID);
        queryDelete.exec();

        response = "0018|DELETE_FOLDER_SUCCESS";
        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }

    }

    if(messagePostProcessing.contains("1012")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        fileDownloadID = responseList.at(responseList.count()-1).toInt();
        QString newName = responseList.at(responseList.count()-2);
        QSqlQuery query;
        query.prepare("SELECT * FROM files WHERE id = :fileID");
        query.bindValue(":fileID", fileDownloadID);
        query.exec();

        int fieldFileName = query.record().indexOf("file_name");
        int fieldFilePath = query.record().indexOf("file_path");
        int fieldFileState = query.record().indexOf("state");
        int fieldFileOwner = query.record().indexOf("owner_id");

        while(query.next()){
            fileDownloadName = query.value(fieldFileName).toString();
            fileDownloadPath = query.value(fieldFilePath).toString();
            fileDownloadState = query.value(fieldFileState).toString();
            fileDownloadOwner = query.value(fieldFileOwner).toInt();
        }

        QFile fileRename(fileDownloadPath);
        if(!fileRename.open(QIODevice::ReadOnly)){
            qDebug() << "Cant open selected file.";
        }

        QString newPath = fileDownloadPath.replace(fileDownloadName,newName);

        if (fileDownloadState == "Private"){
            if (fileDownloadOwner != userID){
                response = "0026|CAN_NOT_ACCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
            else{
                fileRename.rename(newPath);

                QSqlQuery queryRename;
                queryRename.prepare("UPDATE files SET file_name = :fileName, file_path = :filePath WHERE id = :fileID");
                queryRename.bindValue(":fileID", fileDownloadID);
                queryRename.bindValue(":fileName", newName);
                queryRename.bindValue(":filePath", newPath);
                queryRename.exec();

                response = "0025|RENAME_SUCCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }
        else{
            fileRename.rename(newPath);

            QSqlQuery queryRename;
            queryRename.prepare("UPDATE files SET file_name = :fileName, file_path = :filePath WHERE id = :fileID");
            queryRename.bindValue(":fileID", fileDownloadID);
            queryRename.bindValue(":fileName", newName);
            queryRename.bindValue(":filePath", newPath);
            queryRename.exec();

            response = "0025|RENAME_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }



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
        int fieldID = query.record().indexOf("id");
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
                    userID = query.value(fieldID).toInt();
                    qDebug() << userID;
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
        int fieldFilePath = query.record().indexOf("file_path");
        int fieldFileID = query.record().indexOf("id");
        int fieldFolderID = query.record().indexOf("folder_id");
        int fieldFileSize = query.record().indexOf("file_size");
        while (query.next()) {
            QString filePathFromDB = query.value(fieldFilePath).toString();
            QString fileIDFromDB = query.value(fieldFileID).toString();
            QString folderIDFromDB = query.value(fieldFolderID).toString();
            QString fileSizeFromDB = query.value(fieldFileSize).toString();
            response=response + fileIDFromDB + "#" + folderIDFromDB + "#" + fileSizeFromDB + "#" +filePathFromDB+"|";
        }

        response.removeLast();
        qDebug() << response;

        tempDir = rootDir;
        folderCurrentID = 0;

        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
    }

    if(messagePostProcessing.contains("1007")){
        ui->textEdit->append("request");


        QStringList folderNameList = messagePostProcessing.split(u'|');
        QString newFolderName = folderNameList.at(folderNameList.count()-1);
        QString state = folderNameList.at(folderNameList.count()-2);


        QString newFolderPath = tempDir.absoluteFilePath(newFolderName);
        qDebug() << newFolderPath;

        QSqlQuery compareQuery;
        compareQuery.prepare("SELECT * FROM folders WHERE folder_name = :folderName");
        compareQuery.bindValue(":folderName", newFolderName);
        compareQuery.exec();

        int fieldFolderParent = compareQuery.record().indexOf("parent_id");

        bool nameChecked = false;
        while(compareQuery.next()){
            if(compareQuery.value(fieldFolderParent).toInt()==folderCurrentID){
                nameChecked = true;
                response = "0031|FOLDER_EXISTED_IN_CURRENT_FOLDER";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }

        if(nameChecked==false){
            tempDir.mkdir(newFolderName);

            QSqlQuery query;
            query.prepare("INSERT INTO folders(folder_name,folder_path ,state, owner_id, parent_id) "
                          "VALUES (:folderName,:folderPath, :state, :ownerID, :parentID)");
            query.bindValue(":folderName", newFolderName);
            query.bindValue(":state", state);
            query.bindValue(":folderPath", newFolderPath);
            query.bindValue(":ownerID", userID);
            query.bindValue(":parentID", folderCurrentID);

            query.exec();

            response = "0015|CREATE_FOLDER_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1010")){
        ui->textEdit->append("request");
        response = "0023|GET_ALL_FOLDER_SUCCESS|";
        QSqlQuery query("SELECT * FROM folders");
        int fieldFolderPath = query.record().indexOf("folder_path");
        int fieldFolderID = query.record().indexOf("id");
        int fieldParentID = query.record().indexOf("parent_id");
        while (query.next()) {
            QString folderPathFromDB = query.value(fieldFolderPath).toString();
            QString folderIDFromDB = query.value(fieldFolderID).toString();
            QString parentIDFromDB = query.value(fieldParentID).toString();
            response=response + folderIDFromDB + "#" + parentIDFromDB + "#" + folderPathFromDB +"|";
        }

        response.removeLast();
        qDebug() << response;

        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
    }

    if(messagePostProcessing.contains("1011")){
        ui->textEdit->append("request");


        QStringList responseList = messagePostProcessing.split(u'|');
        folderDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM folders WHERE id = :folderID");
        query.bindValue(":folderID", folderDownloadID);
        query.exec();

        int fieldFolderName = query.record().indexOf("folder_name");
        int fieldFolderPath = query.record().indexOf("folder_path");
        int fieldFolderState = query.record().indexOf("state");
        int fieldFolderOwner = query.record().indexOf("owner_id");

        while(query.next()){
            folderDownloadName = query.value(fieldFolderName).toString();
            folderDownloadPath = query.value(fieldFolderPath).toString();
            folderDownloadState = query.value(fieldFolderState).toString();
            folderDonwloadOwner = query.value(fieldFolderOwner).toInt();
        }

        if (folderDownloadState == "Private"){
            if (folderDonwloadOwner != userID){
                response = "0026|CAN_NOT_ACCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
            else{
                tempDir.setPath(folderDownloadPath);
                folderCurrentID = folderDownloadID;

                qDebug() << tempDir;
                response = "0024|MOVE_SUCCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }
        else{
            tempDir.setPath(folderDownloadPath);
            folderCurrentID = folderDownloadID;

            qDebug() << tempDir;
            response = "0024|MOVE_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1014")){
        ui->textEdit->append("request");


        QStringList responseList = messagePostProcessing.split(u'|');
        folderDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM folders WHERE id = :folderID");
        query.bindValue(":folderID", folderDownloadID);
        query.exec();

        int fieldFolderName = query.record().indexOf("folder_name");
        int fieldFolderPath = query.record().indexOf("folder_path");
        int fieldFolderParent = query.record().indexOf("parent_id");

        while(query.next()){
            folderDownloadName = query.value(fieldFolderName).toString();
            folderDownloadPath = query.value(fieldFolderPath).toString();
            folderDownloadParent = query.value(fieldFolderParent).toInt();
        }
        tempDir.setPath(folderDownloadPath);
        folderCurrentID = folderDownloadID;

        qDebug() << tempDir;
        response = "0028|RETURN_SUCCESS|" + QString::number( folderCurrentID) + "|" + QString::number(folderDownloadParent);
        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
    }

    if(messagePostProcessing.contains("1015")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        fileDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM files WHERE id = :fileID");
        query.bindValue(":fileID", fileDownloadID);
        query.exec();

        int fieldFileName = query.record().indexOf("file_name");
        int fieldFilePath = query.record().indexOf("file_path");
        int fieldFileState = query.record().indexOf("state");
        int fieldFileOwner = query.record().indexOf("owner_id");
        int fieldFileSize = query.record().indexOf("file_size");

        while(query.next()){
            fileDownloadName = query.value(fieldFileName).toString();
            fileDownloadPath = query.value(fieldFilePath).toString();
            fileDownloadState = query.value(fieldFileState).toString();
            fileDownloadOwner = query.value(fieldFileOwner).toInt();
            fileDownloadSize = query.value(fieldFileSize).toInt();
        }

        QFile fileCopy(fileDownloadPath);
        if(!fileCopy.open(QIODevice::ReadOnly)){
            qDebug() << "Cant open selected file.";
        }

        QString newPath = tempDir.absolutePath() +"/" + fileDownloadName;
        qDebug() << newPath;



        if (fileDownloadState == "Private"){
            if (fileDownloadOwner != userID){
                response = "0026|CAN_NOT_ACCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
            else{
                fileCopy.copy(newPath);
                QSqlQuery queryCopy;
                queryCopy.prepare("INSERT INTO files (file_name,file_path,state,folder_id,owner_id, file_size) "
                                  "VALUES (:fileName,:filePath,:state,:folderID,:ownerID, :fileSize)");
                queryCopy.bindValue(":fileName", fileDownloadName);
                queryCopy.bindValue(":filePath", newPath);
                queryCopy.bindValue(":state", fileDownloadState);
                queryCopy.bindValue(":folderID", folderCurrentID);
                queryCopy.bindValue(":ownerID", fileDownloadOwner);
                queryCopy.bindValue(":fileSize", fileDownloadSize);
                queryCopy.exec();

                response = "0029|COPY_FILE_SUCCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }
        else{
            fileCopy.copy(newPath);
            QSqlQuery queryCopy;
            queryCopy.prepare("INSERT INTO files (file_name,file_path,state,folder_id,owner_id, file_size) "
                              "VALUES (:fileName,:filePath,:state,:folderID,:ownerID, :fileSize)");
            queryCopy.bindValue(":fileName", fileDownloadName);
            queryCopy.bindValue(":filePath", newPath);
            queryCopy.bindValue(":state", fileDownloadState);
            queryCopy.bindValue(":folderID", folderCurrentID);
            queryCopy.bindValue(":ownerID", fileDownloadOwner);
            queryCopy.bindValue(":fileSize", fileDownloadSize);
            queryCopy.exec();

            response = "0029|COPY_FILE_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1016")){
        ui->textEdit->append("request");

        QStringList responseList = messagePostProcessing.split(u'|');
        fileDownloadID = responseList.at(responseList.count()-1).toInt();
        QSqlQuery query;
        query.prepare("SELECT * FROM files WHERE id = :fileID");
        query.bindValue(":fileID", fileDownloadID);
        query.exec();

        int fieldFileName = query.record().indexOf("file_name");
        int fieldFilePath = query.record().indexOf("file_path");
        int fieldFileState = query.record().indexOf("state");
        int fieldFileOwner = query.record().indexOf("owner_id");
        int fieldFileSize = query.record().indexOf("file_size");

        while(query.next()){
            fileDownloadName = query.value(fieldFileName).toString();
            fileDownloadPath = query.value(fieldFilePath).toString();
            fileDownloadState = query.value(fieldFileState).toString();
            fileDownloadOwner = query.value(fieldFileOwner).toInt();
            fileDownloadSize = query.value(fieldFileSize).toInt();
        }

        QFile fileCut(fileDownloadPath);
        if(!fileCut.open(QIODevice::ReadOnly)){
            qDebug() << "Cant open selected file.";
        }

        QString newPath = tempDir.absolutePath() +"/" + fileDownloadName;
        qDebug() << newPath;



        if (fileDownloadState == "Private"){
            if (fileDownloadOwner != userID){
                response = "0026|CAN_NOT_ACCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
            else{
                fileCut.rename(newPath);
                QSqlQuery queryCut;
                queryCut.prepare("INSERT INTO files (file_name,file_path,state,folder_id,owner_id, file_size) "
                                  "VALUES (:fileName,:filePath,:state,:folderID,:ownerID, :fileSize)");
                queryCut.bindValue(":fileName", fileDownloadName);
                queryCut.bindValue(":filePath", newPath);
                queryCut.bindValue(":state", fileDownloadState);
                queryCut.bindValue(":folderID", folderCurrentID);
                queryCut.bindValue(":ownerID", fileDownloadOwner);
                queryCut.bindValue(":fileSize", fileDownloadSize);
                queryCut.exec();

                QSqlQuery queryDelete;
                queryDelete.prepare("DELETE FROM files WHERE id = :fileID");
                queryDelete.bindValue(":fileID", fileDownloadID);
                queryDelete.exec();

                response = "0030|CUT_FILE_SUCCESS";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }
        else{
            fileCut.rename(newPath);
            QSqlQuery queryCut;
            queryCut.prepare("INSERT INTO files (file_name,file_path,state,folder_id,owner_id, file_size) "
                              "VALUES (:fileName,:filePath,:state,:folderID,:ownerID, :fileSize)");
            queryCut.bindValue(":fileName", fileDownloadName);
            queryCut.bindValue(":filePath", newPath);
            queryCut.bindValue(":state", fileDownloadState);
            queryCut.bindValue(":folderID", folderCurrentID);
            queryCut.bindValue(":ownerID", fileDownloadOwner);
            queryCut.bindValue(":fileSize", fileDownloadSize);
            queryCut.exec();

            QSqlQuery queryDelete;
            queryDelete.prepare("DELETE FROM files WHERE id = :fileID");
            queryDelete.bindValue(":fileID", fileDownloadID);
            queryDelete.exec();

            response = "0030|CUT_FILE_SUCCESS";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }

    if(messagePostProcessing.contains("1017")){
        ui->textEdit->append("request");


        QStringList folderNameList = messagePostProcessing.split(u'|');
        QString newFolderName = folderNameList.at(folderNameList.count()-1);
        QString state = folderNameList.at(folderNameList.count()-2);


        QString newFolderPath = tempDir.absoluteFilePath(newFolderName);
        qDebug() << newFolderPath;

        QSqlQuery compareQuery;
        compareQuery.prepare("SELECT * FROM folders WHERE folder_name = :folderName");
        compareQuery.bindValue(":folderName", newFolderName);
        compareQuery.exec();

        int fieldFolderParent = compareQuery.record().indexOf("parent_id");

        bool nameChecked = false;
        while(compareQuery.next()){
            if(compareQuery.value(fieldFolderParent).toInt()==folderCurrentID){
                nameChecked = true;
                response = "0031|FOLDER_EXISTED_IN_CURRENT_FOLDER";
                foreach (QTcpSocket* socket, clientList) {
                    socket->write(response.toStdString().c_str());
                }
            }
        }

        if(nameChecked==false){
            tempDir.mkdir(newFolderName);

            QSqlQuery query;
            query.prepare("INSERT INTO folders(folder_name,folder_path ,state, owner_id, parent_id) "
                          "VALUES (:folderName,:folderPath, :state, :ownerID, :parentID)");
            query.bindValue(":folderName", newFolderName);
            query.bindValue(":state", state);
            query.bindValue(":folderPath", newFolderPath);
            query.bindValue(":ownerID", userID);
            query.bindValue(":parentID", folderCurrentID);

            query.exec();

            response = "0032|NEW_UPLOAD_FOLDER";
            foreach (QTcpSocket* socket, clientList) {
                socket->write(response.toStdString().c_str());
            }
        }
    }
}


void MainWindow::readSpecialFromSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray messageFromServer = socket->readAll();
    //qDebug() << messageFromServer;
    QString filePath = tempDir.absolutePath() + "/" + fileName;
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Cant open selected file.";
    }
    file.write(messageFromServer);



    fileSize = fileSize - messageFromServer.size();
    ui->textEdit->append(QString::number(fileSize));
    if(fileSize>0){
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialLargeFromSocket()));

    }
    else{
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSpecialFromSocket()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));

        response = "0027|UPLOAD_COMPLETE";
        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
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
        response = "0027|UPLOAD_COMPLETE";
        foreach (QTcpSocket* socket, clientList) {
            socket->write(response.toStdString().c_str());
        }
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



