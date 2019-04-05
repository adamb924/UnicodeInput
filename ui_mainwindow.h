/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "characterwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *textEntry;
    QPushButton *changeFont;
    QVBoxLayout *verticalLayout_2;
    CharacterWidget *characterWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *substringSearch;
    QSpacerItem *horizontalSpacer;
    QLineEdit *hex;
    QLineEdit *glyphName;
    QLabel *glyphNameLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(873, 189);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textEntry = new QLineEdit(centralwidget);
        textEntry->setObjectName(QString::fromUtf8("textEntry"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(20);
        textEntry->setFont(font);

        horizontalLayout->addWidget(textEntry);

        changeFont = new QPushButton(centralwidget);
        changeFont->setObjectName(QString::fromUtf8("changeFont"));

        horizontalLayout->addWidget(changeFont);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        characterWidget = new CharacterWidget(centralwidget);
        characterWidget->setObjectName(QString::fromUtf8("characterWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(characterWidget->sizePolicy().hasHeightForWidth());
        characterWidget->setSizePolicy(sizePolicy2);
        characterWidget->setFrameShape(QFrame::Box);
        characterWidget->setFrameShadow(QFrame::Plain);
        characterWidget->setLineWidth(1);
        characterWidget->setMidLineWidth(0);

        verticalLayout_2->addWidget(characterWidget);


        verticalLayout->addLayout(verticalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        substringSearch = new QCheckBox(centralwidget);
        substringSearch->setObjectName(QString::fromUtf8("substringSearch"));

        horizontalLayout_2->addWidget(substringSearch);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        hex = new QLineEdit(centralwidget);
        hex->setObjectName(QString::fromUtf8("hex"));

        gridLayout->addWidget(hex, 1, 0, 1, 1);

        glyphName = new QLineEdit(centralwidget);
        glyphName->setObjectName(QString::fromUtf8("glyphName"));

        gridLayout->addWidget(glyphName, 1, 1, 1, 1);

        glyphNameLabel = new QLabel(centralwidget);
        glyphNameLabel->setObjectName(QString::fromUtf8("glyphNameLabel"));
        sizePolicy1.setHeightForWidth(glyphNameLabel->sizePolicy().hasHeightForWidth());
        glyphNameLabel->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(glyphNameLabel, 2, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Unicode Input Utility", nullptr));
        changeFont->setText(QApplication::translate("MainWindow", "Change Font...", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Enter a glyph name:", nullptr));
        label->setText(QApplication::translate("MainWindow", "Enter a hex value:", nullptr));
        substringSearch->setText(QApplication::translate("MainWindow", "Search for any substring", nullptr));
        glyphNameLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
