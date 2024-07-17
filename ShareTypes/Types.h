#ifndef types_h
#define types_h

#ifdef __cplusplus
extern "C" {
#endif
  typedef struct MyData {
    float f;
    int i;
  } MyData;
  MyData* CreateStruct();
#ifdef __cplusplus
}
#endif

#endif
