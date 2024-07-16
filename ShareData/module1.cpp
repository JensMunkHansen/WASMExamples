// module1.cpp
#include <emscripten.h>
#include <stdio.h>

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void print_from_module1(int* shared_memory) {
        printf("Module 1: Value from Module 2 is %d\n", shared_memory[0]);
    }
}
