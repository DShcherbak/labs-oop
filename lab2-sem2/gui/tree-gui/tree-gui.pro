QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tree-gui
TEMPLATE = app

CONFIG += c++17

SOURCES += main.cpp\
        choosingtreeform.cpp \
        drawingtree.cpp \
        drawingwidget.cpp \
        drawingwindow.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    choosingtreeform.h \
    drawingtree.h \
    drawingwidget.h \
    drawingwindow.h

FORMS    += mainwindow.ui \
    choosingtreeform.ui \
    drawingwindow.ui
