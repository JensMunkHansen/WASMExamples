#ifndef types_h
#define types_h

#include "Config.h"

#ifdef __cplusplus
extern "C" {
#endif
  typedef struct {
    float f;
    int i;
  } MyData;
  MyData* CreateStruct();
#ifdef __cplusplus
}
#endif

#endif
