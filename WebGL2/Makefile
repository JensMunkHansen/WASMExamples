all:
	emcc webgl_timer.cpp -o webgl_timer.js -s USE_WEBGL2=1 \
    -s EXPORTED_FUNCTIONS="['_isWebGLTimestampAvailable', '_startWebGLTimestampQuery', '_getWebGLTimestamp']" \
    -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" \
    -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web,node
