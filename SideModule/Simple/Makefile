all:
	emcc side.c -s SIDE_MODULE=1 -o side.wasm
	emcc main.c -s MAIN_MODULE=1 -s RUNTIME_LINKED_LIBS="['side.wasm']" -o main.html
