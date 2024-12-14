#!/bin/bash

cur_dir=$(pwd)
cd $cur_dir/SideModule/ES6
source build.sh

cd $cur_dir/Threading
source build.sh wasm
rm $cur_dir/Threading/build_wasm
source build.sh wasm threading

