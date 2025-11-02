TEMPLATE = lib
CONFIG += staticlib c++17
TARGET = Backend

QT += core sql network

SOURCES += backend.cpp \
    Modules/transaction.cpp \
    model.cpp
HEADERS += backend.h \
    Modules/account.h \
    Modules/category.h \
    Modules/currency.h \
    Modules/transaction.h \
    model.h
