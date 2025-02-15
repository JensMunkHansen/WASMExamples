#ifndef VECTOR_ABI_H
#define VECTOR_ABI_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct VectorView VectorView; // Opaque type for the low-level API

  // Callback to call when the memory is updated
  typedef void (*VectorUpdateCallback)(VectorView* view, void* userData);

  // Low-level operations (type-agnostic). We cannot return values (memory space is different)
  void CreateVector(VectorView*& obj, int elementType);
  void DeleteVector(VectorView* view);
  void GetReferenceCounter(VectorView* view, int* referenceCount);
  void SetReferenceCounter(VectorView* view, int);
  void SetDataPointer(VectorView* view, void* data);
  void SetNumberOfTuples(VectorView* view, int nTuples);
  void SetNumberOfComponents(VectorView* view, int nComponents);
  void GetDataPointer(VectorView* view, void** data);
  void GetNumberOfTuples(VectorView* view, int* nTuples);
  void GetNumberOfComponents(VectorView* view, int* nComponents);
  void ShallowCopy(const VectorView* src, VectorView* dest);

  void ResizeVector(VectorView* view, int newCapacity);
  void GetElementType(VectorView* view, int* elementType);

  void SetUpdateCallback(VectorView* view, VectorUpdateCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

#endif // VECTOR_ABI_H
