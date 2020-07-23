#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = VideoPlayer_2
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    mainwindow.cpp \
    videoplayer.cpp \
    login_dialog.cpp \
    config_widget.cpp \
    cJSON.cpp \
    recvthread.cpp

HEADERS  +=videoplayer.h \
    mainwindow.h \
    videoplayer.h \
    login_dialog.h \
    config_widget.h \
    common.h \
    cJSON.h \
    recvthread.h

FORMS    += mainwindow.ui \
    login_dialog.ui \
    config_widget.ui

#ffmpeg
INCLUDEPATH += /usr/include/x86_64-linux-gnu/
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lpthread -lavcodec  -lavformat  -lavutil  -lswscale -lswresample

#cpprest
INCLUDEPATH +=./cpprestsdk/include/
LIBS += -L$$PWD/libs/ -lcpprest -lboost_system -lboost_thread -lboost_chrono -lssl -lcrypto -lpthread

#libevent
INCLUDEPATH += /opt/libevent_x86/include
LIBS +=-L/opt/libevent_x86/lib -levent

#cppjson
#INCLUDEPATH +=./libjson_cpp/include/json
#LIBS +=-L$$PWD/libjson_cpp/libs -l json_cpp
SUBDIRS += \
    VideoPlayer_2.pro

DISTFILES += \
    VideoPlayer_2.pro.user \
    README.md
