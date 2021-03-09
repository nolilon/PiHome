QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/bottoken.h \
    src/idevice.h \
    src/ialarm.h \
    src/imodel.h \
    src/internet.h \
    src/isensor.h \
    src/iview.h \
    src/ivolatile.h \
    src/mainlogic.h \
    src/ports.h \
    src/remotealarm.h \
    src/remotelight.h \
    src/remotelightalarm.h \
    src/remotewethersensors.h \
    src/tcpconnection.h \
    src/telegrambot.h \
    src/telegramobjects.h \
    src/view.h

SOURCES += \
        src/internet.cpp \
        src/main.cpp \
        src/mainlogic.cpp \
        src/remotealarm.cpp \
        src/remotelight.cpp \
        src/remotelightalarm.cpp \
        src/remotewethersensors.cpp \
        src/tcpconnection.cpp \
        src/telegrambot.cpp \
        src/telegramobjects.cpp \
        src/view.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = pihome


