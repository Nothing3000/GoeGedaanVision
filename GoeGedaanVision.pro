 #-------------------------------------------------
#
# Project created by QtCreator 2019-06-04T11:36:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GoeGedaanVision
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

CONFIG += c++11

SOURCES += \
    dice.cpp \
        main.cpp \
        mainwindow.cpp \
    plate.cpp \
        qbwimage.cpp \
        qgrayimage.cpp \
    qhsvimage.cpp

HEADERS += \
    dice.h \
        mainwindow.h \
    plate.h \
        qbwimage.h \
        qgrayimage.h \
    qhsvimage.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
        font/Kenteken.ttf \
        templates/A.bmp \
        templates/B.bmp \
        templates/C.bmp \
        templates/D.bmp \
        templates/E.bmp \
        templates/F.bmp \
        templates/G.bmp \
        templates/H.bmp \
        templates/I.bmp \
        templates/J.bmp \
        templates/K.bmp \
        templates/L.bmp \
        templates/M.bmp \
        templates/N.bmp \
        templates/O.bmp \
        templates/P.bmp \
        templates/Q.bmp \
        templates/R.bmp \
        templates/S.bmp \
        templates/T.bmp \
        templates/U.bmp \
        templates/V.bmp \
        templates/W.bmp \
        templates/X.bmp \
        templates/Y.bmp \
        templates/Z.bmp \
        templates/1.bmp \
        templates/2.bmp \
        templates/3.bmp \
        templates/4.bmp \
        templates/5.bmp \
        templates/6.bmp \
        templates/7.bmp \
        templates/8.bmp \
        templates/9.bmp \
        templates/0.bmp

RESOURCES += \
    kentekenfont.qrc \
    kentekentemplates.qrc

