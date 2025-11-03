TEMPLATE = lib
CONFIG += staticlib c++17
TARGET = Backend

QT += core sql

SOURCES += backend.cpp \
    Modules/category.cpp \
    Modules/currency.cpp \
    Modules/transaction.cpp \
    model.cpp
HEADERS += backend.h \
    Modules/account.h \
    Modules/category.h \
    Modules/currency.h \
    Modules/t.h \
    Modules/transaction.h \
    model.h

PRECOMPILED_HEADER = Modules/pch.h
