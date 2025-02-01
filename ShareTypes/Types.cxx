#include <emscripten/bind.h>

#include "Types.h"
#include "stdlib.h"

MyData* CreateStruct(int _i, float _f)
{
  MyData* p = (MyData*)malloc(sizeof(MyData));
  p->i = _f;
  p->f = _f;
  return p;
}
void DestroyStruct(MyData* s)
{
  free(s);
}

#ifdef CPPTYPES

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
        [](WrappedMyData& obj) -> uintptr_t { return reinterpret_cast<uintptr_t>(&obj); }));
}

#endif
