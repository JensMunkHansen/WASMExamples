#!/bin/bash

cur_dir=$(pwd)

# cd $cur_dir/SideModule/ES6
# source build.sh
# cd $cur_dir/SideModule/ES6/build
# ctest
# 
# cd $cur_dir/SideModule/Simple
# make
# 
# cd $cur_dir/Threading
# source build.sh wasm threading
# cd $cur_dir/Threading/build_wasm
# ctest
# 
# # Does it work in single-configuration???
# cd $cur_dir/VSCodeDebug/ES6
# emcmake cmake --preset default
# cmake --build build --preset Debug

cd $cur_dir/ThreadCallback
source build.sh
cd $cur_dir/ThreadCallback/build
ctest
