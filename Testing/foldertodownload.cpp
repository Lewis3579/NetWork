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
