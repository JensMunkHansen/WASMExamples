#ifndef ARRAY_ABI_H
#define ARRAY_ABI_H

#include "abi.h"
#include "runtime_exports.h"

Runtime_EXTERN_C_BEGIN;

// No types are allowed - only opaque types
typedef struct ArrayView ArrayView;

// Callback to call when the memory is updated
Runtime_EXTERN_C typedef void (*ArrayUpdateCallback)(ArrayView* view, void* userData);

Runtime_EXPORT void ArrayElementSizeGet(int elementType, int* size);
Runtime_EXPORT void ArrayElementTypeGet(const ArrayView* view, int* elementType);

// Low-level operations (type-agnostic).
// We cannot return values (memory space is different)
Runtime_EXPORT void ArrayCreate(ArrayView*& obj, int elementType);
Runtime_EXPORT void ArrayDelete(ArrayView*& view); // TODO: Use reference
Runtime_EXPORT void ArrayIncrementReference(ArrayView* view);

Runtime_EXPORT void ArrayReferenceCounterGet(const ArrayView* view, int* referenceCount);
Runtime_EXPORT void ArrayReferenceCounterSet(ArrayView* view, int);
// Data pointer
Runtime_EXPORT void ArrayDataPointerGet(const ArrayView* view, void** data);
Runtime_EXPORT void ArrayDataPointerSet(ArrayView* view, void* data);
Runtime_EXPORT void ArrayShallowCopy(const ArrayView* src, ArrayView* dest);
// Number of tuples
Runtime_EXPORT void ArrayNumberOfTuplesGet(const ArrayView* view, int* nTuples);
Runtime_EXPORT void ArrayNumberOfTuplesSet(ArrayView* view, int nTuples);
// Number of components
Runtime_EXPORT void ArrayNumberOfComponentsGet(const ArrayView* view, int* nComponents);
Runtime_EXPORT void ArrayNumberOfComponentsSet(ArrayView* view, int nComponents);

Runtime_EXPORT void ArrayResize(ArrayView* view, int newCapacity);

void ArrayUpdateCallbackSet(ArrayView* view, ArrayUpdateCallback callback, void* userData);

Runtime_EXTERN_C_END;

#endif // ARRAY_ABI_H
