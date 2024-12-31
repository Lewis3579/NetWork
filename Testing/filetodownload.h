#ifndef FILETODOWNLOAD_H
#define FILETODOWNLOAD_H
#include <QPushButton>

#include <QObject>

class FileToDownload
{
public:
    FileToDownload();

    void setFileButton(QPushButton* buttonToSet);
    QPushButton* getFileButton();
    QString getFileName();

private:
    QPushButton* button;
};

#endif // FILETODOWNLOAD_H
