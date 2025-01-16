#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDir>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newConnection();
    void readDataFromSocket();
    void readSpecialFromSocket();
    void readSpecialLargeFromSocket();

    void on_pushButton_2_clicked();


private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QList<QTcpSocket*> clientList;

    void addNewClient(QTcpSocket* socket);
    QSqlDatabase db;

    QString response;
    QString fileName;
    int fileSize;
    QDir rootDir;
    int userID;
    int folderCurrentID;
    QDir tempDir;
    QString fileState;

    int fileDownloadID;
    int fileDownloadFolder;
    int fileDownloadOwner;
    QString fileDownloadPath;
    QString fileDownloadName;
    QString fileDownloadState;

    int folderDownloadID;
    int folderDownloadParent;
    int folderDonwloadOwner;
    QString folderDownloadPath;
    QString folderDownloadName;
    QString folderDownloadState;
};
#endif // MAINWINDOW_H
