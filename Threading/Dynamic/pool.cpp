#include <iostream>
#include <emscripten.h>
#include <emscripten/threading.h>

int get_available_threads() {
    if (emscripten_has_threading_support()) {
        int cores = emscripten_num_logical_cores();

        // Handle the case where 0 is returned
        if (cores <= 0) {
            std::cerr << "Warning: Unable to detect logical cores. Using fallback value of 4." << std::endl;
            return 4; // Default fallback value
        }

        return cores;
    } else {
        std::cerr << "Threading is not supported. Assuming single-threaded execution." << std::endl;
        return 1; // Single-threaded fallback
    }
}

int main() {
    int available_threads = get_available_threads();
    std::cout << "Number of available threads: " << available_threads << std::endl;

    return 0;
}
