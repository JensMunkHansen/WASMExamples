emcc module1.cpp -o module1.js \
    -s EXPORTED_FUNCTIONS='["_write_data"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s USE_PTHREADS=1 \
    -s SHARED_MEMORY=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s PROXY_TO_PTHREAD=0 \
    -s ENVIRONMENT='web,node' \
    -pthread \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=0 \
    -s SINGLE_FILE=0

emcc module2.cpp -o module2.js \
    -s EXPORTED_FUNCTIONS='["_read_data"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s USE_PTHREADS=1 \
    -s SHARED_MEMORY=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s PROXY_TO_PTHREAD=0 \
    -s ENVIRONMENT='web,node' \
    -pthread \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=0 \
    -s SINGLE_FILE=0
