#!/usr/bin/bash

qmake-qt5 -o Makefile vkeybd-qt.pro 
make -j $(nproc)

if [ $? -eq 0 ]
then
    ./debug/vkeybd-qt
fi
