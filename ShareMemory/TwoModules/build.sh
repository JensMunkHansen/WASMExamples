emcc module1.cpp -o module1.js \
    -s EXPORTED_FUNCTIONS='["_write_data", "_use_heap", "_read_heap"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s USE_PTHREADS=1 \
    -s SHARED_MEMORY=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s PROXY_TO_PTHREAD=0 \
    -s ENVIRONMENT='web,node,worker' \
    -pthread \
    -s MODULARIZE=1 \
    -s EXPORT_NAME=Module1Factory \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s SINGLE_FILE=0

emcc module2.cpp -o module2.js \
    -s EXPORTED_FUNCTIONS='["_malloc", "_free", "_read_data", "_use_heap"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s USE_PTHREADS=1 \
    -s SHARED_MEMORY=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s PROXY_TO_PTHREAD=0 \
    -s ENVIRONMENT='web,node,worker' \
    -pthread \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s SINGLE_FILE=0

emcc module3.cpp -o module3.js \
     -s EXPORTED_FUNCTIONS='["_custom_malloc", "_custom_free", "_save_heap_state", "_restore_heap_state", "_printHeapInfo", "_sbrk", "_use_heap3", "_read_heap", "_register_function3"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s USE_PTHREADS=1 \
    -s SHARED_MEMORY=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s PROXY_TO_PTHREAD=0 \
    -s ENVIRONMENT='web,node,worker' \
    -pthread \
    -Wl,--wrap=malloc -Wl,--wrap=free \
    -s MODULARIZE=1 \
    -s INITIAL_TABLE=50 \
    -s EXPORT_ALL \
    -s IMPORTED_MEMORY=1 \
    -s EXPORT_NAME=Module3Factory \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s SINGLE_FILE=0

emcc module4.cpp -o module4.js \
     -s EXPORTED_FUNCTIONS='["_custom_malloc", "_custom_free", "_save_heap_state", "_restore_heap_state", "_printHeapInfo", "_sbrk", "_use_heap4", "_register_function4"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s USE_PTHREADS=1 \
    -s SHARED_MEMORY=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s PROXY_TO_PTHREAD=0 \
    -s ENVIRONMENT='web,node,worker' \
    -pthread \
    -Wl,--wrap=malloc -Wl,--wrap=free \
    -s MODULARIZE=1 \
    -s INITIAL_TABLE=50 \
    -s EXPORT_ALL \
    -s IMPORTED_MEMORY=1 \
    -s EXPORT_NAME=Module4Factory \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s SINGLE_FILE=0
