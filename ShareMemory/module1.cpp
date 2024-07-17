#include <emscripten.h>
#include <atomic>
#include <iostream>

// Expose this function to JavaScript
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void set_shared_memory(int* shared_mem, int value) {
        std::atomic<int>* atomic_shared_mem = reinterpret_cast<std::atomic<int>*>(shared_mem);
        atomic_shared_mem->store(value, std::memory_order_relaxed);
        std::cout << "Module 1 set shared memory to: " << value << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void get_shared_memory(int* shared_mem) {
        std::atomic<int>* atomic_shared_mem = reinterpret_cast<std::atomic<int>*>(shared_mem);
        int value = atomic_shared_mem->load(std::memory_order_relaxed);
        std::cout << "Module 1 read shared memory: " << value << std::endl;
    }
}
