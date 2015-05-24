TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_LFLAGS += -static-libgcc

SOURCES += main.cpp \
    cfile.cpp \
    dict.cpp \
    encoder.cpp

HEADERS += \
    cfile.h \
    dict.h \
    dictrestrict.h \
    dictreplace.h \
    includes.h \
    encoder.h \
    MyEx.h \
    censor.h

