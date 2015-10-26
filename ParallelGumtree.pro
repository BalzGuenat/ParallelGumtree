TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    tree.cpp \
    fileparser.cpp \
    matcher.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    tree.h \
    fileparser.h \
    matcher.h

DISTFILES += \
    test_tree

