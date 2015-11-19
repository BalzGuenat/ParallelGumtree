TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += src/main.cpp \
    src/tree.cpp \
    src/fileparser.cpp \
    src/matcher.cpp \
    src/filewriter.cpp \
    src/matchingdumper.cpp \
    src/treecreator.cpp \
    src/mappingstore.cpp \
    src/subtreematcher.cpp \
    src/greedysubtreematcher.cpp \
    src/multimappingstore.cpp \
    src/greedybottomupmatcher.cpp \
    src/treemap.cpp \
    src/classicgumtree.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    src/tree.h \
    src/matcher.h \
    src/filewriter.h \
    src/matchingdumper.h \
    src/treecreator.h \
    src/mappingstore.h \
    src/subtreematcher.h \
    src/greedysubtreematcher.h \
    src/multimappingstore.h \
    src/greedybottomupmatcher.h \
    src/mapping.h \
    src/treemap.h \
    src/fileparser.h \
    src/classicgumtree.h

DISTFILES += \


