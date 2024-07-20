// module2.cpp
#include <emscripten.h>
#include <stdio.h>

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void set_value_in_shared_memory(int* shared_memory) {
        shared_memory[0] = 42;
        printf("Module 2: Set value to 42: %d\n", shared_memory[0]);
    }
}
