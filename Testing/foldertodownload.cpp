#include "foldertodownload.h"

FolderToDownload::FolderToDownload() {}

void FolderToDownload::setFolderButton(QPushButton *buttonToSet)
{
    this->button = buttonToSet;
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
                        + "|" + QString::number(this->parentID);
    qDebug() << dataStore;

    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, QColor(Qt::yellow));
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->update();

    QTextStream out(&file);
    out << dataStore;
}
