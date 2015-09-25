#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T16:29:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CurrentSeedSearch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sss4config.cpp \
    PokeRNG/PokeRNG/Calc5GenSeed.cpp \
    PokeRNG/PokeRNG/CalcOffset.cpp \
    PokeRNG/PokeRNG/DateTime.cpp \
    PokeRNG/PokeRNG/DateTimeIterator.cpp \
    PokeRNG/PokeRNG/DateTimeRange.cpp \
    PokeRNG/PokeRNG/LCG.cpp \
    PokeRNG/PokeRNG/MT.cpp \
    PokeRNG/PokeRNG/Parameters5Gen.cpp \
    PokeRNG/PokeRNG/ROMType.cpp \
    PokeData/BaseStats.cpp \
    PokeData/Pokemon.cpp \
    PokeData/PokemonData.cpp

HEADERS  += mainwindow.h \
    sss4config.h \
    PokeRNG/PokeRNG/Calc5GenSeed.hpp \
    PokeRNG/PokeRNG/CalcOffset.hpp \
    PokeRNG/PokeRNG/Config.hpp \
    PokeRNG/PokeRNG/ConstantParameters5Gen.hpp \
    PokeRNG/PokeRNG/DateTime.hpp \
    PokeRNG/PokeRNG/DateTimeIterator.hpp \
    PokeRNG/PokeRNG/DateTimeRange.hpp \
    PokeRNG/PokeRNG/LCG.hpp \
    PokeRNG/PokeRNG/MT.hpp \
    PokeRNG/PokeRNG/Parameters5Gen.hpp \
    PokeRNG/PokeRNG/PokeRNG.hpp \
    PokeRNG/PokeRNG/ROMType.hpp \
    PokeData/BaseStats.hpp \
    PokeData/PokeData.hpp \
    PokeData/Pokemon.hpp \
    PokeData/PokemonData.hpp

FORMS    += mainwindow.ui

CONFIG += c++11

INCLUDEPATH += ./PokeRNG
