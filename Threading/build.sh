#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 [wasm|linux]"
    exit 1
fi

platform=$(echo "$1" | tr '[:upper:]' '[:lower:]')

if [ "$platform" = "linux" ]; then
    cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DTHREADING_ENABLED=ON -DCMAKE_FIND_ROOT_PATH=/home/jmh/github/Catch2CMake/install
    bear -- bash -c "cmake --build build -j 16"
elif [ "$platform" = "wasm" ]; then
    source "$HOME/github/emsdk/emsdk_env.sh"
#    emcmake cmake -S. -Bbuild_wasm -DCMAKE_BUILD_TYPE=Release
    emcmake cmake -S. -Bbuild_wasm -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DTHREADING_ENABLED=ON -DCMAKE_FIND_ROOT_PATH=/home/jmh/github/Catch2CMake/install
    cmake --build build_wasm -j 16
fi
