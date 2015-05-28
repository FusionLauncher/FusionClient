#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T19:12:51
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FusionClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addgamedialog.cpp \
    watchedfoldersdialog.cpp

HEADERS  += mainwindow.h \
    addgamedialog.h \
    watchedfoldersdialog.h

FORMS    += mainwindow.ui \
    addgamedialog.ui \
    watchedfoldersdialog.ui

unix|win32: LIBS += -lLibFusion

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LibFusion/release/ -lLibFusion
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LibFusion/debug/ -lLibFusion
else:unix: LIBS += -L$$PWD/../LibFusion/ -lLibFusion

INCLUDEPATH += $$PWD/../LibFusion
DEPENDPATH += $$PWD/../LibFusion
