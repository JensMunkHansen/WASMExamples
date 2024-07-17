#!/bin/bash

(
    echo "Emscripten"
    source "$HOME/github/emsdk/emsdk_env.sh"
    emcmake cmake -S. -Bbuild
    cmake --build build
)


