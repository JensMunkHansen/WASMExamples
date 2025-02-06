#include <emscripten.h>
#include <iostream>

// Function to check if WebGL 2.0 and EXT_disjoint_timer_query_webgl2 are available
EM_JS(bool, isWebGLTimestampAvailable, (), {
    if (!Module.gl)
    {
        let canvas = document.createElement('canvas');
        Module.gl = canvas.getContext('webgl2');
        if (!Module.gl)
        {
            console.warn("WebGL 2.0 not supported!");
            return false;
        }

        Module.ext = Module.gl.getExtension("EXT_disjoint_timer_query_webgl2");
        if (!Module.ext)
        {
            console.warn("EXT_disjoint_timer_query_webgl2 not supported!");
            return false;
        }
    }
    return true;
});

// Function to query WebGL timestamp
EM_JS(double, getWebGLTimestamp, (), {
    if (!Module.gl || !Module.ext)
        return -1.0;

    let gl = Module.gl;
    let ext = Module.ext;
    let query = gl.createQuery();

    ext.queryCounterEXT(query, ext.TIMESTAMP_EXT);

    function checkQuery()
    {
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
    }

    return checkQuery();
});

// C++ wrapper function to check WebGL timestamp support
bool checkWebGLTimestampSupport()
{
    return isWebGLTimestampAvailable();
}

// C++ function to get the timestamp value
double queryWebGLTimestamp()
{
    double timestamp = getWebGLTimestamp();
    if (timestamp < 0)
    {
        std::cerr << "Timestamp query not ready or not supported." << std::endl;
    }
    return timestamp;
}

// Main loop
int main()
{
    if (checkWebGLTimestampSupport())
    {
        std::cout << "WebGL timestamp queries are supported." << std::endl;
    }
    else
    {
        std::cout << "WebGL timestamp queries are NOT supported." << std::endl;
        return 1; // Exit if not supported
    }

    emscripten_set_main_loop(
      []()
      {
          double timestamp = queryWebGLTimestamp();
          if (timestamp >= 0)
          {
              std::cout << "WebGL Render Timestamp: " << timestamp << " ns" << std::endl;
          }
      },
      0, 1);

    return 0;
}
