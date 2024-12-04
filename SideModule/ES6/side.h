#ifndef _SIDE_H
#define _SIDE_H

// WebAssembly itself doesn't export types like structs or classes. You must rely
// on ccall, cwrap or embind

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IS_SIDE_MODULE
  struct MyStruct;
#elif IS_MAIN_MODULE
struct MyStruct
{
  float a;
};
#endif
  void CreateMyStruct(struct MyStruct* s);
  float GetValue(struct MyStruct* s);
  void SetValue(struct MyStruct* s);

#ifdef __cplusplus
}
#endif

#endif
