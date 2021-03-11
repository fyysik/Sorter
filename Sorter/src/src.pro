include (../config.pri)

TEMPLATE = app
DESTDIR = ../bin
TARGET = Sorter

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mysort.cpp \
    sortingwidget.cpp
HEADERS += \
    mainwindow.h \
    mysort.h \
    sortingwidget.h
