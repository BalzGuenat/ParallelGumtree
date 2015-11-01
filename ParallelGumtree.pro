TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    tree.cpp \
    fileparser.cpp \
    matcher.cpp \
    filewriter.cpp \
    matchingdumper.cpp \
    treecreator.cpp \
    mappingstore.cpp \
    subtreematcher.cpp \
    greedysubtreematcher.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    tree.h \
    fileparser.h \
    matcher.h \
    filewriter.h \
    matchingdumper.h \
    treecreator.h \
    mappingstore.h \
    subtreematcher.h \
    greedysubtreematcher.h

DISTFILES += \
    test_tree

