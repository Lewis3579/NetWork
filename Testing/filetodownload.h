#ifndef FILETODOWNLOAD_H
#define FILETODOWNLOAD_H
#include <QPushButton>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFileInfo>
#include <QResource>
#include <QObject>
#include <QStyle>

class FileToDownload : public QObject
{

    Q_OBJECT
public:
    FileToDownload();

    void setFileButton(QPushButton* buttonToSet);
    void setFilePath(QString filePath);
    void setFileName(QString fileName);
    void setFileState(QString fileState);
    void setFileID(int fileID);
    void setFolderID(int folderID);
    void setFileSIze(int fileSize);
    QPushButton* getFileButton();
    QString getFileName();
    QString getFileState();
    int getFileID();
    int getFolderID();

public slots:
    void downloadFileFromServer();

private:
    QPushButton* button;
    QString filePath;
    QString fileName;
    QString fileState;
    int fileID;
    int folderID;
    int fileSize;
};

#endif // FILETODOWNLOAD_H
