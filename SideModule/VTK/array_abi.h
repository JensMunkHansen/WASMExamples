#ifndef ARRAY_ABI_H
#define ARRAY_ABI_H

#include "abi.h"

#ifdef __cplusplus
extern "C"
{
#endif

  // No types are allowed - only opaque types
  typedef struct ArrayView ArrayView;

  // Callback to call when the memory is updated
  typedef ABI_FUNC((*ArrayUpdateCallback))(ArrayView* view, void* userData);

  ABI_FUNC(ArrayElementSizeGet)(int elementType, int* size);

  // Low-level operations (type-agnostic).
  // We cannot return values (memory space is different)
  ABI_FUNC(ArrayCreate)(ArrayView*& obj, int elementType);
  ABI_FUNC(ArrayDelete)(ArrayView* view); // TODO: Use reference
  ABI_FUNC(ArrayReferenceCounterGet)(const ArrayView* view, int* referenceCount);
  ABI_FUNC(ArrayReferenceCounterSet)(ArrayView* view, int);
  // Data pointer
  ABI_FUNC(ArrayDataPointerGet)(const ArrayView* view, void** data);
  ABI_FUNC(ArrayDataPointerSet)(ArrayView* view, void* data);
  ABI_FUNC(ArrayShallowCopy)(const ArrayView* src, ArrayView* dest);
  // Number of tuples
  ABI_FUNC(ArrayNumberOfTuplesGet)(const ArrayView* view, int* nTuples);
  ABI_FUNC(ArrayNumberOfTuplesSet)(ArrayView* view, int nTuples);
  // Number of components
  ABI_FUNC(ArrayNumberOfComponentsGet)(const ArrayView* view, int* nComponents);
  ABI_FUNC(ArrayNumberOfComponentsSet)(ArrayView* view, int nComponents);

  ABI_FUNC(ArrayResize)(ArrayView* view, int newCapacity);
  ABI_FUNC(ArrayElementTypeGet)(const ArrayView* view, int* elementType);

  ABI_FUNC(ArrayUpdateCallbackSet)(ArrayView* view, ArrayUpdateCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

#endif // ARRAY_ABI_H
