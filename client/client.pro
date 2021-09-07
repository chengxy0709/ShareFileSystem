#-------------------------------------------------
#
# Project created by QtCreator 2019-11-28T20:45:40
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        tcp_client.cpp \
    tcp_meta.cpp \
    p2p_download.cpp \
    p2p_server.cpp

HEADERS  += tcp_client.h \
    tcp_meta.h \
    p2p_download.h \
    p2p_server.h
