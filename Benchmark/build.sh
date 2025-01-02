#!/bin/bash

(
    source $HOME/github/emsdk/emsdk_env.sh
    emcmake cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release
    cmake --build build -v
)
