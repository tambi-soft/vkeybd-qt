QT  += core gui\
    sql\
    svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#QMAKE_CFLAGS += \
#    -lasound
#    asound
#QMAKE_CXXFLAGS += \
#    -lasound
#    asound
    
LIBS += -lasound \
    -ljack

CONFIG += \
    c++11 \
    console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    button_piano.cpp \
    config.cpp \
    db_adapter.cpp \
    input_keyboard_raw_thread.cpp \
    interface_alsa.cpp \
    interface_audio.cpp \
    interface_jack.cpp \
    keyboard_notes.cpp \
    keyboard_pc.cpp \
    keyboard_piano.cpp \
    main.cpp \
    main_tabs.cpp \
    main_window.cpp \
    menubar.cpp \
    midi_channel_selector.cpp \
    midi_channels_summary.cpp \
    midi_key_selector.cpp \
    midi_pitchwheel.cpp \
    midi_sounds_list.cpp \
    orgelwerk.cpp

HEADERS += \
    button_piano.h \
    config.h \
    db_adapter.h \
    input_keyboard_raw_thread.h \
    interface_alsa.h \
    interface_audio.h \
    interface_jack.h \
    keyboard_notes.h \
    keyboard_pc.h \
    keyboard_piano.h \
    main_tabs.h \
    main_window.h \
    menubar.h \
    midi_channel_selector.h \
    midi_channels_summary.h \
    midi_key_selector.h \
    midi_pitchwheel.h \
    midi_sounds_list.h \
    orgelwerk.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

release:DESTDIR = release
release:OBJECTS_DIR = release/obj
release:MOC_DIR = release/moc
release:RCC_DIR = release/rcc
release:UI_DIR = release/ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/obj
debug:MOC_DIR = debug/moc
debug:RCC_DIR = debug/rcc
debug:UI_DIR = debug/ui

