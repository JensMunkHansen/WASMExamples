#!/bin/bash

(
    source "$HOME/github/emsdk/emsdk_env.sh"
    cmake --preset default
    cmake --build build --config Debug
)
