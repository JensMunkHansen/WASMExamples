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

#cd $cur_dir/ThreadCallback
#source build.sh
#cd $cur_dir/ThreadCallback/build
#ctest

(
    source "$HOME/github/emsdk/emsdk_env.sh"
    bear -- emcmake cmake -S. -Bbuild -Bbuild -DVTK_DIR=/home/jmh/github/VTKCMake/install/lib/cmake/vtk
    bear -- cmake --build build -v
)
