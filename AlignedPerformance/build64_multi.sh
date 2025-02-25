#!/bin/bash

cmake --preset default -DWASM64=ON -DWASMExamples_OPTIMIZATION=BEST -DWASMExamples_DEBUG=NONE
cmake --build build --config Release
