TEMPLATE = app
CONFIG += thread console
QT -= gui
QT += network
TARGET = saslservtest

SOURCES += saslservtest.cpp
include(../examples.pri)