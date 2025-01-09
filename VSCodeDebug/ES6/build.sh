#!/bin/bash

(
    echo "Emscripten"
    source "$HOME/github/emsdk/emsdk_env.sh"
    emcmake cmake --preset default
    cmake --build build --preset Debug
)


