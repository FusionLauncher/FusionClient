#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T19:12:51
#
#-------------------------------------------------

QT       += core gui sql
QT       += network

QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FusionClient
TEMPLATE = app


DESTDIR = ../OUT
MOC_DIR = BUILD
OBJECTS_DIR = BUILD

TRANSLATIONS = FusionLang_de.ts \
    FusionLang_no.ts \
    FusionLang_pl.ts \
    FusionLang_es.ts

RC_ICONS = gfx/Icon.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    addgamedialog.cpp \
    fgamewidget.cpp \
    gameinfodialog.cpp \
    general.cpp \
    gamedbartselectordialog.cpp \
    fsettingsdialog.cpp

HEADERS  += mainwindow.h \
    addgamedialog.h \
    fgamewidget.h \
    gameinfodialog.h \
    general.h \
    gamedbartselectordialog.h \
    fsettingsdialog.h

FORMS    += mainwindow.ui \
    addgamedialog.ui \
    fgamewidget.ui \
    gameinfodialog.ui \
    gamedbartselectordialog.ui \
    fsettingsdialog.ui


LIBS += -lLibFusion
LIBS += -L$$PWD/../OUT -lLibFusion

INCLUDEPATH += $$PWD/../LibFusion/LibFusion
DEPENDPATH += $$PWD/../LibFusion/LibFusion

unix {
    target.path = /usr/bin
    INSTALLS += target
}

DISTFILES += \
    stylesheet.qss \
    dbUpdate

RESOURCES += \
    res.qrc
