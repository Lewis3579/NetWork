#include "filetodownload.h"

FileToDownload::FileToDownload() {}

void FileToDownload::setFileButton(QPushButton* buttonToSet)
{
    this->button = buttonToSet;
    this->button->setStyleSheet("QPushButton[clicked=true]{background-color: red;}"
                                "QPushButton[clicked=false]{background-color: white;}"
                                "QPushButton:hover[access=false]{border-width: 1px;border-style: solid;border-color: white;}");
}

QPushButton* FileToDownload::getFileButton()
{
    return this->button;
}

QString FileToDownload::getFileName()
{
    return this->button->text();
}

QString FileToDownload::getFileState()
{
    return this->fileState;
}

int FileToDownload::getFileID()
{
    return this->fileID;
}

int FileToDownload::getFolderID()
{
    return this->folderID;
}

void FileToDownload::setFilePath(QString filePath){
    this->filePath = filePath;
}

void FileToDownload::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void FileToDownload::setFileState(QString fileState)
{
    this->fileState = fileState;
    if(fileState=="Private"){
        this->button->setProperty("access", false);
        this->button->style()->unpolish(this->button);
        this->button->style()->polish(this->button);
    }
}

void FileToDownload::setFileID(int fileID)
{
    this->fileID = fileID;
}

void FileToDownload::setFolderID(int folderID)
{
    this->folderID = folderID;
}

void FileToDownload::setFileSIze(int fileSize)
{
    this->fileSize = fileSize;
}
void FileToDownload::downloadFileFromServer()
{
    QFile file("D:\\fileINFO.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Error";
        return;
    }
    QString dataStore = this->filePath + "|" + this->fileName + "|" + QString::number(this->fileID)
                        + "|" + QString::number(this->folderID) + "|" + QString::number(this->fileSize) + "|" + this->fileState;
    qDebug() << dataStore;


    QTextStream out(&file);
    out << dataStore;
}
