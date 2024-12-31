/********************************************************************************
** Form generated from reading UI file 'signin.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNIN_H
#define UI_SIGNIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SignIn
{
public:
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *textEdit;

    void setupUi(QDialog *SignIn)
    {
        if (SignIn->objectName().isEmpty())
            SignIn->setObjectName("SignIn");
        SignIn->resize(872, 483);
        lineEdit = new QLineEdit(SignIn);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(260, 190, 211, 31));
        lineEdit_2 = new QLineEdit(SignIn);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(260, 240, 211, 31));
        label = new QLabel(SignIn);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 190, 91, 20));
        label_2 = new QLabel(SignIn);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(170, 240, 63, 20));
        pushButton = new QPushButton(SignIn);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(572, 30, 271, 29));
        pushButton_2 = new QPushButton(SignIn);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(380, 300, 93, 29));
        textEdit = new QTextEdit(SignIn);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(170, 360, 131, 71));

        retranslateUi(SignIn);

        QMetaObject::connectSlotsByName(SignIn);
    } // setupUi

    void retranslateUi(QDialog *SignIn)
    {
        SignIn->setWindowTitle(QCoreApplication::translate("SignIn", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SignIn", "Username", nullptr));
        label_2->setText(QCoreApplication::translate("SignIn", "Pasword", nullptr));
        pushButton->setText(QCoreApplication::translate("SignIn", "Don't have an account? Sign Up", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SignIn", "Sign In", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignIn: public Ui_SignIn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNIN_H
