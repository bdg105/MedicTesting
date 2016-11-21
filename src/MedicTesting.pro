#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T09:43:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedicTesting
TEMPLATE = app


SOURCES += main.cpp\
        winstarttesting.cpp \
    winquestion.cpp

HEADERS  += winstarttesting.h \
    winquestion.h

FORMS    += winstarttesting.ui \
    winquestion.ui

win32: DESTDIR = ../../MedicTesting/bin

unix:   DESTDIR = /Volumes/MUSIC/MedicTesting/bin

QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    res.qrc

RC_FILE = medic_testing_ico.rc
