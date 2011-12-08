#-------------------------------------------------
#
# Project created by QtCreator 2011-11-21T15:02:54
#
#-------------------------------------------------

QT       += core gui sql

TARGET = Smart-Token-Booth-Server
TEMPLATE = app

INCLUDEPATH += ./qserialdevice/src/qserialdevice/
INCLUDEPATH += ./qserialdevice/src/qserialdeviceenumerator/

SOURCES += main.cpp\
        mainwindow.cpp \
    serialconnectwindow.cpp \
    boothserver.cpp \
    boothtoken.cpp \
    boothservice.cpp \
    servicecategory.cpp \
    datamodel.cpp \
    categoryassignwindow.cpp

HEADERS  += mainwindow.h \
    serialconnectwindow.h \
    config.h \
    boothserver.h \
    booth.h \
    boothtoken.h \
    boothservice.h \
    servicecategory.h \
    datamodel.h \
    categoryassignwindow.h \
    packet.h

FORMS    += mainwindow.ui \
    connectwindow.ui \
    categoryassignwindow.ui

CONFIG(debug, debug|release) {
    LIBS += -lqserialdeviced
} else {
    LIBS += -lqserialdevice
}

win32 {
    LIBS += -lsetupapi -luuid -ladvapi32
}
unix:!macx {
    LIBS += -ludev
}

OTHER_FILES += \
    README \
    LICENSE
