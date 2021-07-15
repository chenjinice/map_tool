
QT       += core gui webenginewidgets webchannel network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    common/setting.cpp \
    logwidget/logdock.cpp \
    main.cpp \
    mainwindow/mainwindow.cpp \
    subwindow/normalwindow.cpp \
    subwindow/obuwindow.cpp \
    subwindow/subwindow.cpp \
    subwindow/udpthread.cpp \
    webview/webobject.cpp \
    webview/webview.cpp


HEADERS += \
    common/common_defines.h \
    common/setting.h \
    logwidget/logdock.h \
    mainwindow/mainwindow.h \
    subwindow/normalwindow.h \
    subwindow/obuwindow.h \
    subwindow/subwindow.h \
    subwindow/udpthread.h \
    webview/webobject.h \
    webview/webview.h


INCLUDEPATH += mainwindow/
INCLUDEPATH += webview/
INCLUDEPATH += subwindow/
    img.qrc


INCLUDEPATH += common/
INCLUDEPATH += logwidget/
INCLUDEPATH += libs/zmq/
INCLUDEPATH += libs/protobuf/


LIBS += -lws2_32

RESOURCES += \
