#include <emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <string>
#include <functional>

// Global callback stored as emscripten::val
emscripten::val globalCallback = emscripten::val::null();

// Function to set the callback
void setCallback(emscripten::val callback) {
    if (callback.typeOf().as<std::string>() == "function") {
        globalCallback = callback;
        std::cout << "Callback function set successfully." << std::endl;
    } else {
        std::cerr << "Invalid callback provided. Must be a JavaScript function." << std::endl;
    }
}

// JavaScript worker simulation
void startWorker() {
    EM_ASM({
        console.log("Starting worker...");

        setTimeout(() => {
            const result = "Hello from the worker!";
            // Pass the result to the C++ callback
            Module.ccall('invokeCallback', null, ['string'], [result]);
        }, 1000); // Simulate asynchronous work
    });
}

// Function invoked from JavaScript to call the C++ callback
extern "C" {
EMSCRIPTEN_KEEPALIVE
void invokeCallback(const char* message) {
if (!globalCallback.isNull() && globalCallback.typeOf().as<std::string>() == "function") {
        globalCallback(std::string(message)); // Call the JavaScript callback
    } else {
        std::cerr << "No valid callback set!" << std::endl;
    }
}
}
// Bindings
EMSCRIPTEN_BINDINGS(my_module) {
    using namespace emscripten;
    function("setCallback", &setCallback, allow_raw_pointers());
    function("startWorker", &startWorker);
    function("invokeCallback", &invokeCallback, allow_raw_pointers());
}
