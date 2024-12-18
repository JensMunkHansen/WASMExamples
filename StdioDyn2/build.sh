#!/bin/bash

(
    source "$HOME/github/emsdk/emsdk_env.sh"
    emcmake cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug
    cmake --build build -j 16
)
