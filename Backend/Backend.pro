TEMPLATE = lib
CONFIG += dll c++20
TARGET = Backend
DEFINES += BACKEND_LIB
DESTDIR = debug

QT += core sql

SOURCES += backend.cpp \
    Modules/category.cpp \
    Modules/currency.cpp \
    Modules/transaction.cpp \
    model.cpp
HEADERS += backend.h \
    Modules/account.h \
    Modules/c.h \
    Modules/category.h \
    Modules/currency.h \
    Modules/t.h \
    Modules/transaction.h \
    Modules/utils.h \
    model.h

PRECOMPILED_HEADER = Modules/pch.h
