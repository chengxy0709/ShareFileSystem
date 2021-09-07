#-------------------------------------------------
#
# Project created by QtCreator 2019-11-28T19:17:27
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        tcp_server.cpp \
    tcp_client_socket.cpp \
    server.cpp \
    tcp_meta.cpp \
    format_packet.cpp

HEADERS  += tcp_server.h \
    tcp_client_socket.h \
    server.h \
    tcp_meta.h \
    format_packet.h
