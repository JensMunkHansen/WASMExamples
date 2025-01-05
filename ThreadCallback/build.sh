#!/bin/bash

(
    source "$HOME/github/emsdk/emsdk_env.sh"
    bear -- emcmake cmake -S. -Bbuild -Bbuild -DVTK_DIR=/home/jmh/github/VTKCMake/install/lib/cmake/vtk
    bear -- cmake --build build -v
)
