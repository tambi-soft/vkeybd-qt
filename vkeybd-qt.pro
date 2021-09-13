QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++11 \
    console 
    

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alsa_adapter.cpp \
    button_piano.cpp \
    jack_adapter.cpp \
    keyboard_piano.cpp \
    main.cpp \
    main_window.cpp

HEADERS += \
    alsa_adapter.h \
    button_piano.h \
    jack_adapter.h \
    keyboard_piano.h \
    main_window.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
