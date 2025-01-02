#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include <signin.h>
#include <signup.h>
#include "filetodownload.h"
#include "foldertodownload.h"
#include <QResource>
#include <QIcon>
#include <QTimer>


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
    void on_pushButton_clicked();
    void readDataFromSocket();
    void downloadDataFromSocket();
    void downloadLargeDataFromSocket();
    void readFileDownloaded();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_2_textChanged(const QString &textInLine);


    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::MainWindow *ui;
    void sendData(QString bufferString);
    QString receiveCommand();
public:
    QTcpSocket *socket;
    QString request;
    QString response;
    QByteArray buffer;
    SignIn* signInWindow;
    SignUp* signUpWindow;
    QList<FileToDownload*> fileList;
    QList<FolderToDownload*> folderList;
    QVBoxLayout* scrollLayout;
    QWidget *scrollWidget;


public slots:
    void transitionToSignUp();
    void transitionToSignIn();

private:
    int fileDownloadSize;
    int fileDownloadID;
    int fileDownloadFolder;
    QString fileDownloadPath;
    QString fileDownloadName;
    int currentFolder;

    int folderDownloadID;
    int folderDownloadParent;
    QString folderDownloadPath;
    QString folderDownloadName;
};
#endif // MAINWINDOW_H
