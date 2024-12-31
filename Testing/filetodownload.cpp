#include "filetodownload.h"

FileToDownload::FileToDownload() {}

void FileToDownload::setFileButton(QPushButton* buttonToSet)
{
    this->button = buttonToSet;
    this->button->setMaximumSize(200,100);
}

QPushButton* FileToDownload::getFileButton()
{
    return this->button;
}

QString FileToDownload::getFileName()
{
    return this->button->text();
}
