#!/bin/bash

(
    source $HOME/github/emsdk/emsdk_env.sh
    emcmake cmake -S. -Bbuild
    cmake --build build
)
