#include "foldertodownload.h"

FolderToDownload::FolderToDownload() {}

void FolderToDownload::setFolderButton(QPushButton *buttonToSet)
{
    this->button = buttonToSet;
    this->button->setStyleSheet("QPushButton[clicked=true]{background-color: yellow;}"
                                "QPushButton[clicked=false]{background-color: white;}"
                                "QPushButton:hover[access=false]{border-width: 1px;border-style: solid;border-color: white;}");
}

void FolderToDownload::setFolderPath(QString folderPath)
{
    this->folderPath = folderPath;
}

void FolderToDownload::setFolderName(QString folderName)
{
    this->folderName = folderName;
}

void FolderToDownload::setFolderID(int folderID)
{
    this->folderID = folderID;
}

void FolderToDownload::setParentID(int parentID)
{
    this->parentID = parentID;
}

void FolderToDownload::setFolderState(QString folderState)
{
    this->folderState = folderState;
    if(folderState=="Private"){
        this->button->setProperty("access", false);
        this->button->style()->unpolish(this->button);
        this->button->style()->polish(this->button);
    }
}

QPushButton *FolderToDownload::getFolderButton()
{
    return this->button;
}

QString FolderToDownload::getFolderName()
{
    return this->folderName;
}

int FolderToDownload::getFolderID()
{
    return this->folderID;
}

int FolderToDownload::getParentID()
{
    return this->parentID;
}

void FolderToDownload::downloadFolderFromServer()
{
    QFile file("D:\\folderINFO.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Error";
        return;
    }
    QString dataStore = this->folderPath + "|" + this->folderName + "|" + QString::number(this->folderID)
                        + "|" + QString::number(this->parentID) + "|" + this->folderState;
    qDebug() << dataStore;



    QTextStream out(&file);
    out << dataStore;
}
