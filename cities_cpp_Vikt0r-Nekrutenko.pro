TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE += -O1
QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -Os
QMAKE_CXXFLAGS_RELEASE += -Ofast

SOURCES += \
        main.cpp \
    newSolution/combined_algorithm.cpp \
    # newSolution/genetic_algorithm.cpp

HEADERS += \
    newSolution/combined_algorithm.hpp \
    # newSolution/genetic_algorithm.hpp

INCLUDEPATH += \
    newSolution \
