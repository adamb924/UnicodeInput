# -------------------------------------------------
# Project created by QtCreator 2010-01-12T13:34:56
# -------------------------------------------------
QT += widgets sql
TARGET = UnicodeInput
TEMPLATE = app
SOURCES += main.cpp \
    codepointproxy.cpp \
    mainwindow.cpp \
    characterwidget.cpp \
    databaseadapter.cpp
HEADERS += mainwindow.h \
    characterwidget.h \
    codepointproxy.h \
    databaseadapter.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    resources/UnicodeData.txt

RESOURCES += \
    resources.qrc
