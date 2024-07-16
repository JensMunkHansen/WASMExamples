#include "Types.h"
#include "stdlib.h"

MyData* CreateStruct() {
  MyData* p = (MyData*) malloc(sizeof(MyData));
  p->f = 0.0f;
  p->i = 101;
  return p;
}
