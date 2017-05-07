#-------------------------------------------------
#
# Project created by QtCreator 2016-10-15T17:28:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppRunner
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Runner.cpp \
    AboutDialog.cpp \
    SettingsDialog.cpp \
    Settings.cpp

HEADERS  += MainWindow.h \
    Runner.h \
    AboutDialog.h \
    Version.h \
    SettingsDialog.h \
    Settings.h

FORMS    += MainWindow.ui \
    AboutDialog.ui \
    SettingsDialog.ui

RESOURCES += \
    resources.qrc

RC_FILE += resources.rc
