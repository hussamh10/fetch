/********************************************************************************
** Form generated from reading UI file 'finderwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDERWINDOW_H
#define UI_FINDERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FinderWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *searchBar;
    QScrollArea *scroll_area_container;
    QWidget *scroll_area;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QMainWindow *FinderWindow)
    {
        if (FinderWindow->objectName().isEmpty())
            FinderWindow->setObjectName(QStringLiteral("FinderWindow"));
        FinderWindow->resize(500, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FinderWindow->sizePolicy().hasHeightForWidth());
        FinderWindow->setSizePolicy(sizePolicy);
        FinderWindow->setMaximumSize(QSize(16777215, 400));
        FinderWindow->setFocusPolicy(Qt::WheelFocus);
        FinderWindow->setStyleSheet(QLatin1String("#centralWidget {\n"
"	background: rgba(0,0,0,0.5);\n"
"	border-radius: 5px;\n"
"}"));
        centralWidget = new QWidget(FinderWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        searchBar = new QLineEdit(centralWidget);
        searchBar->setObjectName(QStringLiteral("searchBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(searchBar->sizePolicy().hasHeightForWidth());
        searchBar->setSizePolicy(sizePolicy2);
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        searchBar->setFont(font);
        searchBar->setStyleSheet(QLatin1String("background: rgba(0,0,0,0.8);\n"
"color: white;\n"
"padding: 10px;\n"
"border-radius: 5px;"));
        searchBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        searchBar->setClearButtonEnabled(false);

        verticalLayout->addWidget(searchBar);

        scroll_area_container = new QScrollArea(centralWidget);
        scroll_area_container->setObjectName(QStringLiteral("scroll_area_container"));
        scroll_area_container->setFocusPolicy(Qt::NoFocus);
        scroll_area_container->setStyleSheet(QLatin1String("#scroll_area_container {\n"
"background: transparent;\n"
"border: none;\n"
"}"));
        scroll_area_container->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_area_container->setWidgetResizable(true);
        scroll_area_container->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        scroll_area = new QWidget();
        scroll_area->setObjectName(QStringLiteral("scroll_area"));
        scroll_area->setGeometry(QRect(0, 0, 482, 329));
        sizePolicy1.setHeightForWidth(scroll_area->sizePolicy().hasHeightForWidth());
        scroll_area->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI"));
        font1.setPointSize(12);
        scroll_area->setFont(font1);
        scroll_area->setStyleSheet(QLatin1String("#scroll_area {\n"
"	background: transparent;\n"
"	border-radius: 5px;\n"
"}\n"
"QPushButton {\n"
"	padding: 10px;\n"
"	background: rgba(0,0,0,0.5);\n"
"	color: white;\n"
"	margin: 0px;\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: rgba(0,0,0,1);\n"
"}\n"
"\n"
"QPushButton:focus {\n"
"	background: rgba(255,255,255,1);\n"
"	color: black;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(scroll_area);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        scroll_area_container->setWidget(scroll_area);

        verticalLayout->addWidget(scroll_area_container);

        FinderWindow->setCentralWidget(centralWidget);
        QWidget::setTabOrder(searchBar, scroll_area_container);

        retranslateUi(FinderWindow);

        QMetaObject::connectSlotsByName(FinderWindow);
    } // setupUi

    void retranslateUi(QMainWindow *FinderWindow)
    {
        FinderWindow->setWindowTitle(QApplication::translate("FinderWindow", "Fuzzy Finder", nullptr));
        searchBar->setPlaceholderText(QApplication::translate("FinderWindow", "\360\237\224\215 Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FinderWindow: public Ui_FinderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDERWINDOW_H
