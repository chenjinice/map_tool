
QT       += core gui webenginewidgets webchannel

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
    main.cpp \
    common/setting.cpp \
    widget/logdock.cpp \
    widget/mainwindow.cpp \
    widget/obuwindow.cpp \
    widget/subwindow.cpp \
    webview/webobject.cpp \
    webview/webview.cpp \
    obu/obuudp.cpp \
    common/convert_cv2x_2020/convert.cpp \
    common/convert_cv2x_2020/convert_bsm.cpp \
    common/convert_cv2x_2020/convert_map.cpp \
    common/convert_cv2x_2020/convert_rsi.cpp \
    common/convert_cv2x_2020/convert_rsm.cpp \
    common/convert_cv2x_2020/convert_spat.cpp \
    common/local/local.cpp \
    obu/msg/spat_func.cpp \
    obu/msg/rsm_func.cpp \
    obu/msg/rsi_func.cpp \
    obu/msg/msg.cpp \
    obu/msg/map_func.cpp \
    obu/msg/bsm_func.cpp \
    obu/device/this_device.cpp \
    obu/application/application.cpp \
    obu/application/v2v/app_icw.cpp \
    obu/application/v2v/app_fcw.cpp \
    obu/application/v2v/app_avw.cpp \
    obu/application/v2i/app_vrucw.cpp \
    obu/application/v2i/app_slw.cpp \
    common/my/my_common.cpp

HEADERS += \
    obu/obuudp.h \
    widget/logdock.h \
    widget/mainwindow.h \
    widget/obuwindow.h \
    widget/subwindow.h \
    webview/webobject.h \
    webview/webview.h \
    common/convert_cv2x_2020/convert.h \
    common/local/gb_defines.h \
    common/local/local.h \
    obu/msg/msg.h \
    obu/msg/map_func.h \
    obu/msg/bsm_func.h \
    obu/device/this_device.h \
    obu/application/application.h


INCLUDEPATH += widget/
INCLUDEPATH += obu/
INCLUDEPATH += obu/msg/
INCLUDEPATH += obu/device/
INCLUDEPATH += obu/application/
INCLUDEPATH += webview/
INCLUDEPATH += subwindow/


INCLUDEPATH += common/
INCLUDEPATH += common/local/
INCLUDEPATH += common/convert_cv2x_2020/
INCLUDEPATH += logwidget/
INCLUDEPATH += libs/ltev2020/include/


LIBS += -lpthread
LIBS += -L$$PWD/libs/ltev2020/ -lcv2x

RESOURCES +=


