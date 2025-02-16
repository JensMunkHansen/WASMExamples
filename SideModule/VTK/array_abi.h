#ifndef ARRAY_ABI_H
#define ARRAY_ABI_H

#ifdef __cplusplus
extern "C"
{
#endif

  // No types are allowed - only opaque types
  typedef struct ArrayView ArrayView;

  // Callback to call when the memory is updated
  typedef void (*ArrayUpdateCallback)(ArrayView* view, void* userData);

  void ArrayElementSizeGet(int elementType, int* size);

  // Low-level operations (type-agnostic).
  // We cannot return values (memory space is different)
  void ArrayCreate(ArrayView*& obj, int elementType); // *& not supported
  void ArrayDelete(ArrayView* view);                  // TODO: Use reference
  void ArrayReferenceCounterGet(const ArrayView* view, int* referenceCount);
  void ArrayReferenceCounterSet(ArrayView* view, int);
  // Data pointer
  void ArrayDataPointerGet(const ArrayView* view, void** data);
  void ArrayDataPointerSet(ArrayView* view, void* data);
  void ArrayShallowCopy(const ArrayView* src, ArrayView* dest);
  // Number of tuples
  void ArrayNumberOfTuplesGet(const ArrayView* view, int* nTuples);
  void ArrayNumberOfTuplesSet(ArrayView* view, int nTuples);
  // Number of components
  void ArrayNumberOfComponentsGet(const ArrayView* view, int* nComponents);
  void ArrayNumberOfComponentsSet(ArrayView* view, int nComponents);

  void ArrayResize(ArrayView* view, int newCapacity);
  void ArrayElementTypeGet(const ArrayView* view, int* elementType);

  void ArrayUpdateCallbackSet(ArrayView* view, ArrayUpdateCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

#endif // ARRAY_ABI_H
