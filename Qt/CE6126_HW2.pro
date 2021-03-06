#-------------------------------------------------
#
# Project created by QtCreator 2018-05-20T11:06:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CE6126_HW2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    Canvas.cpp \
    CarObject.cpp \
    CaseDataLoader.cpp \
    GA_ThreadRunner.cpp \
    GeneticAlgorithm.cpp \
    MainWindow.cpp \
    RBFN.cpp \
    SystemDataCenter.cpp \
    TrainDataLoader.cpp

HEADERS += \
        MainWindow.h \
    AllConstant.hpp \
    AllEnumerate.h \
    AllStructure.h \
    Canvas.h \
    CarObject.hpp \
    CaseDataLoader.h \
    GA_ThreadRunner.h \
    GeneticAlgorithm.h \
    Particule.h \
    RBFN.h \
    SystemDataCenter.h \
    TrainDataLoader.h
