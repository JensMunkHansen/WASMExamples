all:
	emcc -pthread example.c -o example.html -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1 -s PTHREAD_POOL_SIZE_STRICT -s PTHREAD_POOL_SIZE=4 
