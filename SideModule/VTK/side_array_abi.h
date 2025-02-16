#ifndef SIDE_ARRAY_ABI_H
#define SIDE_ARRAY_ABI_H

#include "abi.h"
#include "array_abi.h"

#ifdef __cplusplus
extern "C"
{
#endif
  ABI_FUNC(Process)(struct ArrayView* pVector);
#ifdef __cplusplus
}
#endif

#endif
