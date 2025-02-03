all:
	emcc mycode.c -s PTHREAD_POOL_SIZE=2 -s USE_PTHREADS=1 -s SHARED_MEMORY=1 -s INITIAL_MEMORY=64MB -s EXPORTED_FUNCTIONS="['_myFunction']" -s EXPORTED_RUNTIME_METHODS="['cwrap']" -sEXPORT_ES6=1 -o mymodule.mjs


