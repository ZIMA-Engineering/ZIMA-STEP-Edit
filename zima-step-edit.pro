#-------------------------------------------------
#
# Project created by QtCreator 2012-02-24T19:13:26
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ZIMA-STEP-Edit
TEMPLATE = app

win32:INCLUDEPATH += ../
VPATH += ./src

SOURCES += main.cpp\
        mainwindow.cpp \
    filefinderthread.cpp \
    fileeditthread.cpp \
    settingsdialog.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    filefinderthread.h \
    fileeditthread.h \
    settingsdialog.h \
    aboutdialog.h \
    zima-step-edit.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    aboutdialog.ui

RESOURCES += \
    zima-step-edit.qrc

win32:CONFIG += static
win32:RC_FILE = src/zima-step-edit.rc

OTHER_FILES += \
    zima-step-edit.rc \
    README \
    LICENSE \
    AUTHORS

CODECFORTR = UTF-8
TRANSLATIONS += locale/ZIMA-STEP-Edit_cs_CZ.ts
