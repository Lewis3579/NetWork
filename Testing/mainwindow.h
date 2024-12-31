#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include <signin.h>
#include <signup.h>
#include "filetodownload.h"

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
    void readFileDownloaded();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_2_textChanged(const QString &textInLine);

    void downloadFromServer();
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
    QVBoxLayout* scrollLayout;
    QWidget *scrollWidget;


public slots:
    void transitionToSignUp();
    void transitionToSignIn();
};
#endif // MAINWINDOW_H
