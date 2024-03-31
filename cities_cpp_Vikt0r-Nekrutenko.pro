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
    newSolution/ant_colony_algorithm.hpp \
    newSolution/combined_algorithm.hpp \
    newSolution/dfs_algorithm.hpp

INCLUDEPATH += \
    newSolution \
