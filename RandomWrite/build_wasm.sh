#!/bin/bash

mkdir -p build_wasm
cd build_wasm
source ~/github/emsdk/emsdk_env.sh
emcmake -S.. -B. -DCMAKE_BUILD_TYPE=Release
make -j16
