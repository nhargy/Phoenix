#!/bin/bash

echo $PATH

rm -rf build
mkdir build
cd build

cmake .. 

make -j4

QT_QPA_PLATFORM=xcb ./AmBeCube-EXE
