#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T15:01:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = SimpleRacer
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cc\
        MainWindow.cc \
    GameLogic.cc \
    Common.cc \
    RenderingWidget.cc \
    PhysicsObject.cc \
    ArtificialRacer.cc \
    InputController.cc \
    SimpleRacer.cc \
    DelaySimulator.cc \
    ContactListener.cc \
    StatisticsEngine.cc \
    Settings.cc \
    Testing.cc \
    NetworkEngine.cc \
    SurveyEngine.cc \
    ScreenRecorder.cc


HEADERS  += MainWindow.hh \
    GameLogic.hh \
    Common.hh \
    RenderingWidget.hh \
    PhysicsObject.hh \
    ArtificialRacer.hh \
    InputController.hh \
    SimpleRacer.hh \
    DelaySimulator.hh \
    ContactListener.hh \
    StatisticsEngine.hh \
    Settings.hh \
    Testing.hh \
    NetworkEngine.hh \
    SurveyEngine.hh \
    ScreenRecorder.hh

FORMS    += MainWindow.ui

RESOURCES += \
    rendering.qrc \
    htdocs.qrc \
    conditions.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Box2D/lib/ -lBox2d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Box2D/libd/ -lBox2d

INCLUDEPATH += $$PWD/Box2D/include
DEPENDPATH += $$PWD/Box2D/include

unix:!macx: LIBS += -L$$PWD/Box2D/lib/ -lBox2D

INCLUDEPATH += $$PWD/Box2D/include
DEPENDPATH += $$PWD/Box2D/include

unix:!macx: PRE_TARGETDEPS += $$PWD/Box2D/lib/libBox2D.a

OTHER_FILES +=
