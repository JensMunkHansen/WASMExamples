#include <emscripten/bind.h>

#include "Types.h"
#include "stdlib.h"

MyData* CreateStruct()
{
  MyData* p = (MyData*)malloc(sizeof(MyData));
  p->f = 0.0f;
  p->i = 101;
  return p;
}

#ifdef CPPTYPES

// Embind bindings
EMSCRIPTEN_BINDINGS(my_data)
{
  // Bind MyStruct as a value type
  emscripten::value_object<MyData>("MyData")
    .field("i", &MyData::i)
    .field("f", &MyData::f);
  emscripten::class_<WrappedMyData>("WrappedMyData")
    .constructor<int, float>()
    .property("i", &WrappedMyData::getInt, &WrappedMyData::setInt)
    .property("f", &WrappedMyData::getFloat, &WrappedMyData::setFloat);
}

#endif
