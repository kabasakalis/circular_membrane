/********************************************************************************
** Form generated from reading UI file 'membrane.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMBRANE_H
#define UI_MEMBRANE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Membrane
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Membrane)
    {
        if (Membrane->objectName().isEmpty())
            Membrane->setObjectName(QString::fromUtf8("Membrane"));
        Membrane->resize(400, 300);
        menuBar = new QMenuBar(Membrane);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        Membrane->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Membrane);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Membrane->addToolBar(mainToolBar);
        centralWidget = new QWidget(Membrane);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Membrane->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Membrane);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Membrane->setStatusBar(statusBar);

        retranslateUi(Membrane);

        QMetaObject::connectSlotsByName(Membrane);
    } // setupUi

    void retranslateUi(QMainWindow *Membrane)
    {
        Membrane->setWindowTitle(QApplication::translate("Membrane", "Membrane", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Membrane: public Ui_Membrane {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEMBRANE_H
