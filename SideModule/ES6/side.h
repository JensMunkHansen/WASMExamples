#ifndef _SIDE_H
#define _SIDE_H

// WebAssembly itself doesn't export types like structs or classes. You must rely
// on ccall, cwrap or embind

#ifdef __cplusplus
extern "C"
{
#endif
  struct MyStruct;
  void CreateMyStruct(struct MyStruct* s);

#ifdef __cplusplus
}
#endif

#endif
