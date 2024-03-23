TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        sources/edge.cpp \
        sources/matrix.cpp \
        sources/row.cpp \
        sources/vertex.cpp

HEADERS += \
    headers/colony.hpp \
    headers/edge.hpp \
    headers/matrix.hpp \
    headers/vertex.hpp \
    headers/row.hpp

INCLUDEPATH += \
    headers \
    sources
