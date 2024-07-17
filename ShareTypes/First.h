#ifndef first_h
#define first_h

#include "Types.h"
#ifndef CPP_TYPES
#ifdef __cplusplus
extern "C" {
#endif
#endif
  MyData Produce();
  void Consume(MyData* data);
#ifdef __cplusplus
#ifndef CPP_TYPES
}
#endif  
#endif

#endif
