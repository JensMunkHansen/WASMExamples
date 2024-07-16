all:
	emcc module1.cpp -s EXPORTED_FUNCTIONS='["_print_from_module1"]' -s EXPORTED_RUNTIME_METHODS='["print", "printErr", "cwrap", "ccall"]' -o module1.js
	emcc module2.cpp -s EXPORTED_FUNCTIONS='["_set_value_in_shared_memory"]' -s EXPORTED_RUNTIME_METHODS='["print", "printErr", "cwrap", "ccall"]' -o module2.js
