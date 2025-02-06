#include <emscripten.h>
#include <iostream>

extern "C"
{
    // Check if WebGL 2.0 and EXT_disjoint_timer_query_webgl2 are available
    EMSCRIPTEN_KEEPALIVE
    bool isWebGLTimestampAvailable()
    {
        return EM_ASM_INT({
            if (!Module.gl)
            {
                let canvas = document.createElement('canvas');
                Module.gl = canvas.getContext('webgl2');
                if (!Module.gl)
                {
                    console.warn("WebGL 2.0 not supported!");
                    return 0;
                }

                Module.ext = Module.gl.getExtension("EXT_disjoint_timer_query_webgl2");
                if (!Module.ext)
                {
                    console.warn("EXT_disjoint_timer_query_webgl2 not supported!");
                    return 0;
                }
            }
            return 1;
        });
    }

    // Start a WebGL timestamp query
    EMSCRIPTEN_KEEPALIVE
    void startWebGLTimestampQuery()
    {
        EM_ASM({
            if (!Module.gl || !Module.ext)
                return;

            let gl = Module.gl;
            let ext = Module.ext;

            if (!Module.query)
            {
                Module.query = gl.createQuery();
            }

            gl.clearColor(0.0, 0.0, 0.0, 1.0);
            gl.clear(gl.COLOR_BUFFER_BIT);

            ext.queryCounterEXT(Module.query, ext.TIMESTAMP_EXT);

            gl.flush(); // Ensure commands are pushed to GPU
        });
    }

    // Retrieve the WebGL timestamp result
    EMSCRIPTEN_KEEPALIVE
    double getWebGLTimestamp()
    {
        return EM_ASM_DOUBLE({
            if (!Module.gl || !Module.ext || !Module.query)
                return -1.0;

            let gl = Module.gl;
            let ext = Module.ext;
            let query = Module.query;

            let available = gl.getQueryParameter(query, gl.QUERY_RESULT_AVAILABLE);
            let disjoint = gl.getParameter(ext.GPU_DISJOINT_EXT);

            if (available && !disjoint)
            {
                let timestamp = gl.getQueryParameter(query, gl.QUERY_RESULT);
                console.log("GPU Timestamp: " + timestamp);
                return timestamp;
            }
            else
            {
                return -1.0; // Query not ready yet
            }
        });
    }

} // extern "C"
