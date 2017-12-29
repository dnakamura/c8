#!/bin/sh

set -evx

mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=g++-5 -DCMAKE_C_COMPILER=gcc-5 \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
     ../
make
