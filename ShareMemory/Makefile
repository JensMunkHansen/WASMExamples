all:
	/bin/bash -c "source $(HOME)/github/emsdk/emsdk_env.sh"
	emcc module1.cpp -o module1.js -s EXPORTED_FUNCTIONS='["_set_shared_memory", "_get_shared_memory"]' -s MODULARIZE=1 -s EXPORT_ES6=1 -s EXPORT_NAME="Module1" -s USE_PTHREADS=1 -s SHARED_MEMORY=1  -s INITIAL_MEMORY=64MB  -s ENVIRONMENT='node,worker' -s PTHREAD_POOL_SIZE=4 #-sTABLE_BASE=0 -sSIDE_MODULE=1
	emcc module2.cpp -o module2.js -s EXPORTED_FUNCTIONS='["_get_shared_memory", "_get_shared_memory"]' -s MODULARIZE=1 -s EXPORT_ES6=1 -s EXPORT_NAME="Module2" -s USE_PTHREADS=1 -s SHARED_MEMORY=1 -s INITIAL_MEMORY=64MB -s ENVIRONMENT='node,worker'  -s PTHREAD_POOL_SIZE=4 # -sTABLE_BASE=100 -sSIDE_MODULE=1
