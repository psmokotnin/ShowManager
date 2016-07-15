#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T23:58:02
#
#-------------------------------------------------

QT       += core gui multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets opengl avwidgets

TARGET = ShowManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        smapplication.cpp\
        Action/smaction.cpp\
        Action/smactionlist.cpp \
        Action/smactionempty.cpp \
        Action/smactionaudio.cpp \
        smview.cpp \
    Action/smactionplay.cpp \
    Action/smactionstop.cpp \
    Action/smactionopacity.cpp \
    Action/smactionwait.cpp \
    Action/smactionvideonative.cpp \
    Action/smactionvideo.cpp

HEADERS  += mainwindow.h\
            smapplication.h\
            Action/smaction.h\
            Action/smactionlist.h \
            Action/smactionempty.h \
            Action/smactionaudio.h \
            smview.h \
            todo.h \
    Action/smactionplay.h \
    Action/smactionstop.h \
    Action/smactionopacity.h \
    Action/smactionwait.h \
    Action/smactionvideonative.h \
    Action/smactionvideo.h

FORMS    += mainwindow.ui \
    smaction.ui

DISTFILES += \
    README.md

RESOURCES += \
    images.qrc