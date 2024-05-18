/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *ip_edit;
    QLineEdit *port_edit;
    QPushButton *connect_button;
    QLineEdit *send_edit;
    QLineEdit *receive_edit;
    QLabel *label;
    QPushButton *disconnect_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *giveup_button;
    QPushButton *move_button;
    QPushButton *ready_button;
    QPushButton *ai_button;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800,500);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ip_edit = new QLineEdit(centralwidget);
        ip_edit->setObjectName(QString::fromUtf8("ip_edit"));
        ip_edit->setGeometry(QRect(30, 30, 111, 20));
        ip_edit->setAlignment(Qt::AlignCenter);
        port_edit = new QLineEdit(centralwidget);
        port_edit->setObjectName(QString::fromUtf8("port_edit"));
        port_edit->setGeometry(QRect(160, 30, 51, 20));
        port_edit->setAlignment(Qt::AlignCenter);
        connect_button = new QPushButton(centralwidget);
        connect_button->setObjectName(QString::fromUtf8("connect_button"));
        connect_button->setGeometry(QRect(230, 30, 80, 20));
        send_edit = new QLineEdit(centralwidget);
        send_edit->setObjectName(QString::fromUtf8("send_edit"));
        send_edit->setGeometry(QRect(30, 60, 181, 20));
        receive_edit = new QLineEdit(centralwidget);
        receive_edit->setObjectName(QString::fromUtf8("receive_edit"));
        receive_edit->setGeometry(QRect(30, 90, 181, 20));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(230, 90, 81, 16));
        label->setAlignment(Qt::AlignCenter);
        label_2=new QLabel(centralwidget);
        label_2->setAlignment(Qt::AlignCenter);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(500, 125,100,100));
        label_3=new QLabel(centralwidget);
        label_3->setAlignment(Qt::AlignCenter);
        label_3->setObjectName(QString::fromUtf8("label_2"));
        label_3->setGeometry(QRect(500, 125,200,200));
        giveup_button = new QPushButton(centralwidget);
        giveup_button->setObjectName(QString::fromUtf8("giveup_button"));
        giveup_button->setGeometry(QRect(230, 30, 80, 20));
        move_button = new QPushButton(centralwidget);
        move_button->setObjectName(QString::fromUtf8("move_button"));
        move_button->setGeometry(QRect(230, 30, 80, 20));
        ready_button = new QPushButton(centralwidget);
        ready_button->setObjectName(QString::fromUtf8("ready_button"));
        ready_button->setGeometry(QRect(230, 30, 80, 20));
        ai_button = new QPushButton(centralwidget);
        ai_button->setObjectName(QString::fromUtf8("ready_button"));
        ai_button->setGeometry(QRect(230, 30, 80, 20));
        disconnect_button = new QPushButton(centralwidget);
        disconnect_button->setObjectName(QString::fromUtf8("disconnect_button"));
        disconnect_button->setGeometry(QRect(30, 120, 281, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 346, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
      MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        connect_button->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Receive", nullptr));
        disconnect_button->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "倒计时", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "玩家信息", nullptr));
        giveup_button->setText(QCoreApplication::translate("MainWindow", "RESIGN_OP", nullptr));
        move_button->setText(QCoreApplication::translate("MainWindow", "MOVE_OP", nullptr));
        ready_button->setText(QCoreApplication::translate("MainWindow", "READY_OP", nullptr));
        ai_button->setText(QCoreApplication::translate("MainWindow", "AI", nullptr));
        ai_button->move(700,175);
        ready_button->move(700,100);
        giveup_button->move(700,150);
        move_button->move(700,125);
        label_2->move(500,125);
        label_3->move(500,225);
        connect_button->move(700,25);
        label->move(700,75);
        disconnect_button->move(500,50);
        ip_edit->move(500,25);
        send_edit->move(500,100);
        receive_edit->move(500,75);
        port_edit->move(625,25);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
