#include "filetodownload.h"

FileToDownload::FileToDownload() {}

void FileToDownload::setFileButton(QPushButton* buttonToSet)
{
    this->button = buttonToSet;
}

QPushButton* FileToDownload::getFileButton()
{
    return this->button;
}

QString FileToDownload::getFileName()
{
    return this->button->text();
}

void FileToDownload::setFilePath(QString filePath){
    this->filePath = filePath;
}

void FileToDownload::setFileName(QString fileName)
{
    this->fileName = fileName;
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
void FileToDownload::downloadFromServer()
{

    QFile file("D:\\fileINFO.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Error";
        return;
    }
    QString dataStore = this->filePath + "|" + this->fileName + "|" + QString::number(this->fileID)
                        + "|" + QString::number(this->folderID) + "|" + QString::number(this->fileSize);
    qDebug() << dataStore;

    QTextStream out(&file);
    out << dataStore;
}
