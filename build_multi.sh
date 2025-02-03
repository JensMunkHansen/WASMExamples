#!/bin/bash

cmake --preset default -DWASMExamples_DEBUG=SOURCE_MAPS -DWASMExamples_OPTIMIZATION=BEST
cmake --build build --config Release
