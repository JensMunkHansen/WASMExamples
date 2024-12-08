all:
	emcc vector_lookup.c -o vector_lookup.html -s WASM=2 -s EXPORTED_FUNCTIONS="['_main']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -s INITIAL_MEMORY=134217728 -s MAXIMUM_MEMORY=2147483648 -s ALLOW_MEMORY_GROWTH=1 -msimd128


run:
	emrun --no_browser --port 8080 .
