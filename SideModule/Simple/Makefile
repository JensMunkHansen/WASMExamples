all:
	emcc side.c -s SIDE_MODULE=2 -s EXPORTED_FUNCTIONS="['_side', '_CreateMyStruct']" -o side.wasm
	emcc main.c -s MAIN_MODULE=2 side.wasm -o main.html
