#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T15:01:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleRacer
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cc\
        MainWindow.cc \
    GameLogic.cc \
    Common.cc \
    RenderingWidget.cc

HEADERS  += MainWindow.hh \
    GameLogic.hh \
    Common.hh \
    RenderingWidget.hh

FORMS    += MainWindow.ui

RESOURCES += \
    car.qrc
