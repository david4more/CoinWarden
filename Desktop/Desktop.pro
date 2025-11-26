QT       += core gui sql widgets network
CONFIG += c++20

SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

# Backend setup
INCLUDEPATH += $$PWD/../Backend
LIBS += $$OUT_PWD/../Backend/debug/Backend.lib

# QCustomPlot setup
INCLUDEPATH += $$PWD/QCustomPlot
LIBS += $$PWD/../Desktop/QCustomPlot/qcustomplotd2.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
