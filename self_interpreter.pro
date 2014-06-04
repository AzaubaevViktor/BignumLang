TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Wall

SOURCES += main.c \
    statemachine.c \
    bignum.c \
    errors.c \
    test.c \
    parser.c \
    regs.c

HEADERS += \
    statemachine.h \
    bignum.h \
    errors.h \
    test.h \
    parser.h \
    regs.h

