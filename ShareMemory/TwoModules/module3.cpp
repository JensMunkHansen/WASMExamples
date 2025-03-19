#include <emscripten.h>
#include <stdint.h>
#include <stdio.h>

extern char __heap_base;

#define HEAP_OFFSET 0x100000 // 1MB offset

uintptr_t heap_end = 0;

extern "C" EMSCRIPTEN_KEEPALIVE void* custom_sbrk(intptr_t increment)
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

extern "C" EMSCRIPTEN_KEEPALIVE void printHeapInfo()
{
  printf("Heap base: %p\n", &__heap_base);
  printf("Heap current: %p\n", (void*)heap_end);
}

#if 0
// -Wl,--wrap=malloc -Wl,--wrap=free

void* __wrap_malloc(size_t size) {
    return custom_malloc(size);
}

void __wrap_free(void* ptr) {
    custom_free(ptr);
}

#endif
