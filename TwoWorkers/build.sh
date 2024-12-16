#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 [wasm|linux] threading"
    exit 1
fi

platform=$(echo "$1" | tr '[:upper:]' '[:lower:]')

threading=$(echo "$2" | tr '[:upper:]' '[:lower:]')
echo ${threading}
if [ "$platform" = "linux" ]; then
    if [ $# -eq 2 ]; then
        cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release -DTHREADING_ENABLED=ON
    else
        cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release -DTHREADING_ENABLED=OFF
    fi
    bear -- bash -c "cmake --build build -j 16"
elif [ "$platform" = "wasm" ]; then
    source "$HOME/github/emsdk/emsdk_env.sh"
    if [ "$threading" = "threading" ]; then
        emcmake cmake -S. -Bbuild_wasm -DCMAKE_BUILD_TYPE=Release -DTHREADING_ENABLED=ON
    else
        emcmake cmake -S. -Bbuild_wasm -DCMAKE_BUILD_TYPE=Release -DTHREADING_ENABLED=OFF
    fi
    cmake --build build_wasm -j 16
fi