#include <emscripten/bind.h>

#include "Types.h"
#include "stdio.h"
#include "stdlib.h"
#include <cstdint>

MyData* CreateStruct(int _i, float _f)
{
  MyData* p = (MyData*)malloc(sizeof(MyData));
  p->i = _i;
  p->f = _f;
  return p;
}
void DestroyStruct(MyData* s)
{
  if (s)
  {
    printf("Freeing struct at address: %0zx\n", reinterpret_cast<uintptr_t>(s));
    free(s);
    s = nullptr;
  }
}

#ifdef CPPTYPES
int WrappedMyData::StaticInt = 0;
// Static method for creating object from pointer
WrappedMyData* WrappedMyData::fromPointer(uintptr_t ptr)
{
  return reinterpret_cast<WrappedMyData*>(ptr);
}

// Embind bindings
EMSCRIPTEN_BINDINGS(my_data)
{
  // Bind MyStruct as a value type
  emscripten::value_object<MyData>("MyData").field("i", &MyData::i).field("f", &MyData::f);
  emscripten::class_<WrappedMyData>("WrappedMyData")
    .constructor<int, float>()
    .property("i", &WrappedMyData::getInt, &WrappedMyData::setInt)
    .property("f", &WrappedMyData::getFloat, &WrappedMyData::setFloat)
    .function("getPointer",
      emscripten::optional_override(
        [](WrappedMyData& obj) -> uintptr_t { return reinterpret_cast<uintptr_t>(&obj); }))
    .class_property("StaticInt", &WrappedMyData::StaticInt)
    .class_function("fromPointer", &WrappedMyData::fromPointer,
      emscripten::allow_raw_pointers()); // Static function
}

#endif
