#!/bin/bash

(
    echo "Emscripten"
    source "$HOME/github/emsdk/emsdk_env.sh"
    bear -- emcmake cmake --preset default
    bear -- cmake --build build --config Debug -v
)


