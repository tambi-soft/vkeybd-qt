#!/usr/bin/bash

qmake-qt5 -o Makefile vkeybd-qt.pro --cflags --libs alsa
make -j $(nproc)

if [ $? -eq 0 ]
then
    ./debug/vkeybd-qt
fi
