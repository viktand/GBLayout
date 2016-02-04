#-------------------------------------------------
#
# Project created by QtCreator 2016-01-17T15:51:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GBLayout
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newform.cpp \
    avLabel.cpp \
    entertext.cpp \
    insets.cpp \
    actions.cpp \
    qavbutton.cpp \
    qavcheckbox.cpp

HEADERS  += mainwindow.h \
    newform.h \
    avLabel.h \
    entertext.h \
    insets.h \
    actions.h \
    qavbutton.h \
    qavcheckbox.h

FORMS    += mainwindow.ui \
    newform.ui \
    entertext.ui \
    insets.ui \
    actions.ui

RESOURCES += \
    res/resurses.qrc

DISTFILES +=
