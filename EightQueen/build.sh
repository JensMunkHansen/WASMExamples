#!/bin/bash

emcmake cmake -Bbuild -S. -DCMAKE_BUILD_TYPE=Release
cmake --build build
