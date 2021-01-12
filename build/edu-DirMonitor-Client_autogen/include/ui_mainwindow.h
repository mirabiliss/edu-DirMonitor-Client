/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionConnect;
    QAction *action_txt;
    QAction *action_docx;
    QAction *action_bin;
    QAction *action_zip;
    QAction *action_png;
    QAction *action_jpg;
    QAction *action_mp3;
    QAction *action_gif;
    QAction *action_cpp;
    QAction *action_tmp;
    QAction *action_pdf;
    QAction *actionAdd_custom;
    QAction *actionChoose_path;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *path_label;
    QLabel *totalSize_label;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuExtensions;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1042, 601);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QFont font;
        actionExit->setFont(font);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        action_txt = new QAction(MainWindow);
        action_txt->setObjectName(QString::fromUtf8("action_txt"));
        action_txt->setCheckable(true);
        action_docx = new QAction(MainWindow);
        action_docx->setObjectName(QString::fromUtf8("action_docx"));
        action_docx->setCheckable(true);
        action_bin = new QAction(MainWindow);
        action_bin->setObjectName(QString::fromUtf8("action_bin"));
        action_bin->setCheckable(true);
        action_zip = new QAction(MainWindow);
        action_zip->setObjectName(QString::fromUtf8("action_zip"));
        action_zip->setCheckable(true);
        action_png = new QAction(MainWindow);
        action_png->setObjectName(QString::fromUtf8("action_png"));
        action_png->setCheckable(true);
        action_jpg = new QAction(MainWindow);
        action_jpg->setObjectName(QString::fromUtf8("action_jpg"));
        action_jpg->setCheckable(true);
        action_mp3 = new QAction(MainWindow);
        action_mp3->setObjectName(QString::fromUtf8("action_mp3"));
        action_mp3->setCheckable(true);
        action_gif = new QAction(MainWindow);
        action_gif->setObjectName(QString::fromUtf8("action_gif"));
        action_gif->setCheckable(true);
        action_cpp = new QAction(MainWindow);
        action_cpp->setObjectName(QString::fromUtf8("action_cpp"));
        action_cpp->setCheckable(true);
        action_tmp = new QAction(MainWindow);
        action_tmp->setObjectName(QString::fromUtf8("action_tmp"));
        action_tmp->setCheckable(true);
        action_pdf = new QAction(MainWindow);
        action_pdf->setObjectName(QString::fromUtf8("action_pdf"));
        action_pdf->setCheckable(true);
        actionAdd_custom = new QAction(MainWindow);
        actionAdd_custom->setObjectName(QString::fromUtf8("actionAdd_custom"));
        actionChoose_path = new QAction(MainWindow);
        actionChoose_path->setObjectName(QString::fromUtf8("actionChoose_path"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        verticalLayout->addLayout(gridLayout);

        path_label = new QLabel(centralwidget);
        path_label->setObjectName(QString::fromUtf8("path_label"));

        verticalLayout->addWidget(path_label);

        totalSize_label = new QLabel(centralwidget);
        totalSize_label->setObjectName(QString::fromUtf8("totalSize_label"));

        verticalLayout->addWidget(totalSize_label);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1042, 22));
        QFont font1;
        font1.setPointSize(12);
        menubar->setFont(font1);
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuExtensions = new QMenu(menubar);
        menuExtensions->setObjectName(QString::fromUtf8("menuExtensions"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuExtensions->menuAction());
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionChoose_path);
        menuFile->addAction(actionExit);
        menuExtensions->addAction(action_txt);
        menuExtensions->addAction(action_docx);
        menuExtensions->addAction(action_pdf);
        menuExtensions->addAction(action_bin);
        menuExtensions->addAction(action_zip);
        menuExtensions->addAction(action_png);
        menuExtensions->addAction(action_jpg);
        menuExtensions->addAction(action_mp3);
        menuExtensions->addAction(action_gif);
        menuExtensions->addAction(action_cpp);
        menuExtensions->addAction(action_tmp);
        menuExtensions->addAction(actionAdd_custom);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Directory Monitor", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+W", nullptr));
#endif // QT_CONFIG(shortcut)
        actionConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        action_txt->setText(QCoreApplication::translate("MainWindow", ".txt", nullptr));
        action_docx->setText(QCoreApplication::translate("MainWindow", ".docx / .doc", nullptr));
        action_bin->setText(QCoreApplication::translate("MainWindow", ".bin", nullptr));
        action_zip->setText(QCoreApplication::translate("MainWindow", ".zip", nullptr));
        action_png->setText(QCoreApplication::translate("MainWindow", ".png", nullptr));
        action_jpg->setText(QCoreApplication::translate("MainWindow", ".jpg / .jpeg", nullptr));
        action_mp3->setText(QCoreApplication::translate("MainWindow", ".mp3", nullptr));
        action_gif->setText(QCoreApplication::translate("MainWindow", ".gif", nullptr));
        action_cpp->setText(QCoreApplication::translate("MainWindow", ".cpp", nullptr));
        action_tmp->setText(QCoreApplication::translate("MainWindow", ".tmp", nullptr));
        action_pdf->setText(QCoreApplication::translate("MainWindow", ".pdf", nullptr));
        actionAdd_custom->setText(QCoreApplication::translate("MainWindow", "Add custom...", nullptr));
        actionChoose_path->setText(QCoreApplication::translate("MainWindow", "Choose path", nullptr));
        path_label->setText(QString());
        totalSize_label->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuExtensions->setTitle(QCoreApplication::translate("MainWindow", "Extensions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
