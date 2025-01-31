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

catch2dir="$HOME/github/Catch2CMake/install/lib/cmake/Catch2"
vtkdir="$HOME/github/VTKCMake/install/lib/cmake/vtk"
if [[ ! -d "$catch2dir"  || ! -d "$vtkdir" ]]; then
    echo "Directory does not exist."
    exit 0
fi

(
    source "$HOME/github/emsdk/emsdk_env.sh"
    bear -- emcmake cmake -S. -Bbuild -Bbuild -DVTK_DIR=$vtkdir -DCatch2_DIR=$catch2dir
    bear -- cmake --build build -j 16
)
