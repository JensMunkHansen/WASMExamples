#include <emscripten.h>
#include <stdint.h>
#include <stdio.h>

extern char __heap_base;

namespace
{
static float* staticData = nullptr;
}

#define HEAP_OFFSET 0x200000 // 1MB offset

uintptr_t heap_end = 0;
uintptr_t previous_heap = 0;

void* custom_sbrk(intptr_t increment)
{
  if (heap_end == 0)
  {
    heap_end = (uintptr_t)&__heap_base + HEAP_OFFSET;
  }

  uintptr_t prev_heap_end = heap_end;
  heap_end += increment;

  EM_ASM({ console.log("custom_sbrk: Allocating " + $0 + " bytes, heap_end now at: " + $1); },
    increment, heap_end);

  // Call JavaScript to grow memory if needed
  EM_ASM(
    {
      if ($0 >= HEAP8.length)
      {
        console.log("Growing WebAssembly Memory...");
        Module.wasmMemory.grow(1); // Grow by 64KB (1 page)
      }
    },
    heap_end);

  return (void*)prev_heap_end;
}

extern "C" EMSCRIPTEN_KEEPALIVE void* custom_malloc(size_t size)
{
  return custom_sbrk(size);
}

extern "C" EMSCRIPTEN_KEEPALIVE void custom_free(void* ptr)
{
  // No-op for now, or implement a free list
}

// Save Heap Pointer Before Switching Modules
extern "C" EMSCRIPTEN_KEEPALIVE void save_heap_state()
{
  previous_heap = heap_end;
}

// Restore Heap Pointer After Switching Back
extern "C" EMSCRIPTEN_KEEPALIVE void restore_heap_state()
{
  heap_end = previous_heap;
}

extern "C" EMSCRIPTEN_KEEPALIVE void printHeapInfo()
{
  printf("Heap base: %p\n", &__heap_base);
  printf("Heap current: %p\n", (void*)heap_end);
}

extern "C" EMSCRIPTEN_KEEPALIVE void* __wrap_malloc(size_t size)
{
  return custom_malloc(size);
}

extern "C" EMSCRIPTEN_KEEPALIVE void __wrap_free(void* ptr)
{
  custom_free(ptr);
}

extern "C" EMSCRIPTEN_KEEPALIVE void* malloc(size_t size)
{
  return __wrap_malloc(size); // Redirect all malloc calls to wrap_malloc
}

extern "C" EMSCRIPTEN_KEEPALIVE void free(void* ptr)
{
  __wrap_free(ptr); // Ensure `free()` is also wrapped
}

extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  volatile void use_heap4()
  {
    printf("module4\n");
    staticData = (float*)custom_sbrk(3 * sizeof(float));
    staticData[0] = 10.0f;
    staticData[1] = 11.0f;
    staticData[2] = 12.0f;
  }

  EMSCRIPTEN_KEEPALIVE void* register_function4()
  {
    return (void*)use_heap4;
  }
}

// TODO: Save heap state....
