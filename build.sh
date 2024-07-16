#!/bin/bash

(
    echo "Emscripten"
    source "$HOME/github/emsdk/emsdk_env.sh"
    emcmake cmake -S. -Bbuild -DVTK_DIR=/home/jmh/github/VTKCMake/install/lib/cmake/vtk
    cmake --build build
)


