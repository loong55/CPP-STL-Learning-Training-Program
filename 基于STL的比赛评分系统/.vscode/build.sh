#!/bin/bash
if [ ! -d "build" ]; then
    mkdir build;
else
    rm -rf build/*
fi
cd build
#创建build目录，放置CMake文件
cmake ..
make