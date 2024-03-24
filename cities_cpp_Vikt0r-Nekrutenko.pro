TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \

HEADERS += \
    newSolution/ant_colony_algorithm.hpp \
    newSolution/dfs_algorithm.hpp

INCLUDEPATH += \
    newSolution \
