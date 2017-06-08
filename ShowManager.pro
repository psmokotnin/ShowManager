#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T23:58:02
#
#-------------------------------------------------

QT       += core gui multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets opengl

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
    Action/smactionvideonative.cpp

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

#CONFIG += qtav
#CONFIG += vlc


CONFIG(qtav) {
    message(Video actions by qtav)
    greaterThan(QT_MAJOR_VERSION, 4): QT += avwidgets
    DEFINES += "VIDEO_QTAV=1"
    SOURCES += Action/smactionvideoqtav.cpp
    HEADERS += Action/smactionvideoqtav.h
} else {
CONFIG(vlc) {
    message(Video actions by vlc)
    DEFINES += "VIDEO_VLC=1"

    #libs for Windows
    win32-msvc* {
        LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC\sdk\lib" #if you are at windows os
        LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC" # dll
        INCLUDEPATH += "C:\Program Files (x86)\VideoLAN\VLC\sdk\include"
    }

    SOURCES += Action/smactionvideovlc.cpp
    HEADERS += Action/smactionvideovlc.h

    unix|win32: LIBS += -llibvlc
}
else {
    message(Video actions by native qt multimedia)
}
}
FORMS    += mainwindow.ui \
    smaction.ui

DISTFILES += \
    README.md

RESOURCES += \
    images.qrc
