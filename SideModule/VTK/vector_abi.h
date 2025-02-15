#ifndef VECTOR_ABI_H
#define VECTOR_ABI_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct VectorView VectorView; // Opaque type for the low-level API

  // Low-level operations (type-agnostic)
  VectorView* CreateVector(int elementType); // elementType is an integer representing the data type
  void DestroyVector(VectorView* view);
  int GetReferenceCounter(VectorView* view);
  void SetReferenceCounter(VectorView* view, int);
  void SetDataPointer(VectorView* view, void* data);
  void SetNumberOfTuples(VectorView* view, int nTuples);
  void SetNumberOfComponents(VectorView* view, int nComponents);
  void* GetDataPointer(VectorView* view);
  int GetNumberOfTuples(VectorView* view);
  int GetNumberOfComponents(VectorView* view);
  void ShallowCopy(const VectorView* src, VectorView* dest);

  void ResizeVector(VectorView* view, int newCapacity);
  int GetElementType(VectorView* view); // Returns the integer representing the type

  // Callback to call when the memory is updated
  typedef void (*VectorUpdateCallback)(VectorView* view, void* userData);

  void SetUpdateCallback(VectorView* view, VectorUpdateCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

#endif // VECTOR_ABI_H
