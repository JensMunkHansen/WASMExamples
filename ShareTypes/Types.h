#ifndef types_h
#define types_h

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct MyData
  {
    float f;
    int i;
  } MyData;
  MyData* CreateStruct();

#ifdef __cplusplus
}
#endif

#ifdef CPPTYPES
struct WrappedMyData
{
  MyData data;
  // Constructor to initialize values
  WrappedMyData(int _i, float _f)
  {
    data.i = _i;
    data.f = _f;
  }
  // Getters & Setters
  int getInt() const { return data.i; }
  void setInt(int value) { data.i = value; }

  float getFloat() const { return data.f; }
  void setFloat(float value) { data.f = value; }
};
#endif

#endif
