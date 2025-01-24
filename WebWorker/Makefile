all:
	emcc add.c -s EXPORTED_FUNCTIONS="['_add']" -s EXPORTED_RUNTIME_METHODS="['cwrap']" -s MODULARIZE=1 -s EXPORT_ES6=1 -o add.mjs
