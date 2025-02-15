#include "vector_abi.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct VectorView
{
  void* data;
  int nTuples;
  int nComponents;
  int capacity;
  int elementSize;
  int elementType; // Conforms to VTK type values

  int referenceCount;
  VectorUpdateCallback updateCallback;
  void* userData;
};

// Helper function to get the size of each element based on VTK elementType
static int GetElementSize(int elementType)
{
  switch (elementType)
  {
    case 10:
      return sizeof(float); // VTK_FLOAT
    case 11:
      return sizeof(double); // VTK_DOUBLE
    case 2:
      return sizeof(char); // VTK_CHAR
    case 3:
      return sizeof(unsigned char); // VTK_UNSIGNED_CHAR
    case 4:
      return sizeof(short); // VTK_SHORT
    case 5:
      return sizeof(unsigned short); // VTK_UNSIGNED_SHORT
    case 6:
      return sizeof(int); // VTK_INT
    case 7:
      return sizeof(unsigned int); // VTK_UNSIGNED_INT
    case 12:
      return sizeof(long long); // VTK_LONG_LONG
    case 13:
      return sizeof(unsigned long long); // VTK_UNSIGNED_LONG_LONG
    default:
      return 0; // Unsupported type
  }
}

VectorView* CreateVector(int elementType)
{
  int elementSize = GetElementSize(elementType);
  if (elementSize == 0)
  {
    printf("Unsupported element type: %d\n", elementType);
    return nullptr;
  }

  VectorView* view = (VectorView*)malloc(sizeof(VectorView));
  if (!view)
  {
    printf("Memory allocation for VectorView failed.\n");
    return nullptr;
  }

  view->data = nullptr;
  view->nTuples = 0;
  view->nComponents = 1;
  view->capacity = 0;
  view->elementSize = elementSize;
  view->elementType = elementType;
  view->updateCallback = nullptr;
  view->userData = nullptr;
  view->referenceCount = 1;
  return view;
}

int GetReferenceCounter(VectorView* view)
{
  return view->referenceCount;
}
void SetReferenceCounter(VectorView* view, int count)
{
  view->referenceCount = count;
}

void IncrementReference(VectorView* view)
{
  if (view)
  {
    view->referenceCount++;
  }
}

void DecrementReference(VectorView* view)
{
  printf("Decrementing counter %d\n", view->referenceCount);
  if (view)
  {
    view->referenceCount--;
    if (view->referenceCount == 0)
    {
      if (view->data)
      {

        free(view->data);
      }
      free(view);
    }
  }
}

void DestroyVector(VectorView* view)
{
  if (view)
  {
    DecrementReference(view);
  }
}

void SetDataPointer(VectorView* view, void* data)
{
  view->data = data;
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void SetNumberOfTuples(VectorView* view, int nTuples)
{
  if (nTuples > view->capacity)
  {
    ResizeVector(view, nTuples);
  }
  view->nTuples = nTuples;
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void SetNumberOfComponents(VectorView* view, int nComponents)
{
  view->nComponents = nComponents;
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void* GetDataPointer(VectorView* view)
{
  return view->data;
}

int GetNumberOfTuples(VectorView* view)
{
  return view->nTuples;
}

int GetNumberOfComponents(VectorView* view)
{
  return view->nComponents;
}

void ResizeVector(VectorView* view, int newCapacity)
{
  if (view->elementSize == 0)
  {
    printf("Element size not set.\n");
    return;
  }

  if (newCapacity <= view->capacity)
  {
    return;
  }

  void* newData = realloc(view->data, newCapacity * view->elementSize * view->nComponents);
  if (!newData)
  {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  view->data = newData;
  view->capacity = newCapacity;

  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

int GetElementType(VectorView* view)
{
  return view->elementType;
}

void ShallowCopy(VectorView* dest, const VectorView* src)
{
  if (!dest || !src)
  {
    printf("ShallowCopy: Null vector passed.\n");
    return;
  }

  dest->data = src->data;
  dest->nTuples = src->nTuples;
  dest->nComponents = src->nComponents;
  dest->capacity = src->capacity;
  dest->elementSize = src->elementSize;
  dest->elementType = src->elementType;
}

void SetUpdateCallback(VectorView* view, VectorUpdateCallback callback, void* userData)
{
  if (!view)
    return;
  view->updateCallback = callback;
  view->userData = userData;
}
