#!/bin/bash

(
    source ~/github/emsdk/emsdk_env.sh
    emcmake cmake -S. -Bbuild
    cmake --build build -j 16
)
