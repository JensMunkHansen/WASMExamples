all:
	emcc First.cxx  Types.cxx -o First.js -sEXPORTED_FUNCTIONS='["_CreateStruct", "_Produce", "_Consume"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s EXPORT_ES6=1 -s MODULARIZE=1 -sEXPORT_NAME=loadFirstModule
	emcc First.cxx Types.cxx -o Second.js -sEXPORTED_FUNCTIONS='["_CreateStruct", "_Produce", "_Consume"]' -sEXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -sEXPORT_ES6=1 -s MODULARIZE=1 -sEXPORT_NAME=loadSecondModule
