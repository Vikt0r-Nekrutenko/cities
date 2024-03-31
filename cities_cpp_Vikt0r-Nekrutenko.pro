TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE += -O1
QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES += \
        main.cpp \

HEADERS += \
    newSolution/combined_algorithm.hpp \
    newSolution/genetic_algorithm.hpp

INCLUDEPATH += \
    newSolution \
