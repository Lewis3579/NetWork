/********************************************************************************
** Form generated from reading UI file 'signup.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUP_H
#define UI_SIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SignUp
{
public:
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QTextEdit *textEdit;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *SignUp)
    {
        if (SignUp->objectName().isEmpty())
            SignUp->setObjectName("SignUp");
        SignUp->resize(796, 476);
        lineEdit = new QLineEdit(SignUp);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(210, 160, 211, 31));
        lineEdit_2 = new QLineEdit(SignUp);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(210, 230, 211, 31));
        textEdit = new QTextEdit(SignUp);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(170, 330, 104, 75));
        label = new QLabel(SignUp);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 170, 91, 20));
        label_2 = new QLabel(SignUp);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(120, 230, 71, 20));
        pushButton = new QPushButton(SignUp);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(330, 290, 93, 29));
        pushButton_2 = new QPushButton(SignUp);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(472, 30, 291, 29));

        retranslateUi(SignUp);

        QMetaObject::connectSlotsByName(SignUp);
    } // setupUi

    void retranslateUi(QDialog *SignUp)
    {
        SignUp->setWindowTitle(QCoreApplication::translate("SignUp", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SignUp", "Username", nullptr));
        label_2->setText(QCoreApplication::translate("SignUp", "Password", nullptr));
        pushButton->setText(QCoreApplication::translate("SignUp", "Sign Up", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SignUp", "Already have an account? Sign In", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignUp: public Ui_SignUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUP_H
