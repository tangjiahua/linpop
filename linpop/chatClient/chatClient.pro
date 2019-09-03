#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T20:13:44
#
#-------------------------------------------------

QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatClient
TEMPLATE = app

LIBS += -lpthread
SOURCES += main.cpp\
    login.cpp \
    connectserver.cpp \
    register.cpp \
    recvmsgthread.cpp \
    mainwindow.cpp \
    mulitemdelegate.cpp \
    talkbox.cpp \
    cnctthread.cpp \
    recvthread.cpp

HEADERS  += \
    login.h \
    connectserver.h \
    register.h \
    recvmsgthread.h \
    mainwindow.h \
    mulitemdelegate.h \
    mulistitemdata.h \
    talkbox.h \
    cnctthread.h \
    recvthread.h

FORMS    += interface.ui \
    login.ui \
    register.ui \
    mainwindow.ui \
    talkbox.ui

RESOURCES += \
    resources.qrc \
    qq.qrc

DISTFILES += \
    ../../../图片/timg.jpeg
