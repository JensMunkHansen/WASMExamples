#ifndef first_h
#define first_h

#include "Types.h"
#ifdef __cplusplus
extern "C" {
#endif
  MyData Produce();
  void Consume(MyData* data);
#ifdef __cplusplus
}
#endif

#endif
