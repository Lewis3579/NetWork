#ifndef FOLDERTODOWNLOAD_H
#define FOLDERTODOWNLOAD_H

#include <QPushButton>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFileInfo>
#include <QResource>
#include <QObject>

#include <QObject>

class FolderToDownload: public QObject
{
    Q_OBJECT
public:
    FolderToDownload();
    void setFolderButton(QPushButton* buttonToSet);
    void setFolderPath(QString folderPath);
    void setFolderName(QString folderName);
    void setFolderID(int folderID);
    void setParentID(int parentID);
    QPushButton* getFolderButton();
    QString getFolderName();
    int getFolderID();
    int getParentID();

public slots:
    void downloadFolderFromServer();

private:
    QPushButton* button;
    QString folderPath;
    QString folderName;
    int folderID;
    int parentID;
};

#endif // FOLDERTODOWNLOAD_H
