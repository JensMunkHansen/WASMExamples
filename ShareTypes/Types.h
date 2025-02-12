#ifndef types_h
#define types_h

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct MyData
  {
    int i;
    float f;
  } MyData;
  MyData* CreateStruct(int _i, float _f);
  void DestroyStruct(MyData* s);
#ifdef __cplusplus
}
#endif

#ifdef CPPTYPES
#include <cstdint>
struct WrappedMyData
{
  static int StaticInt;
  MyData data;
  // Constructor to initialize values
  WrappedMyData(int _i, float _f)
  {
    data.i = _i;
    data.f = _f;
  }
  // Getters & Setters
  int getInt() const { return data.i; }
  void setInt(int value)
  {
    data.i = value;
    WrappedMyData::StaticInt = value;
  }

  float getFloat() const { return data.f; }
  void setFloat(float value) { data.f = value; }

  // Static method for creating object from pointer
  static WrappedMyData* fromPointer(uintptr_t ptr);
};
#endif

#endif
