#include "array_abi.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct ArrayView
{
  void* data;
  int nTuples;
  int nComponents;
  int capacity; // TODO: Do this efficiently
  int elementSize;
  int elementType; // Conforms to VTK type values
  int referenceCount;
  ArrayUpdateCallback updateCallback;
  void* userData;
  void* clientData;
};

// Helper function to get the size of each element based on VTK elementType
static int ArrayElementSizeGet(int elementType)
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

void ArrayCreate(ArrayView*& view, int elementType)
{
  int elementSize = ArrayElementSizeGet(elementType);
  if (elementSize == 0)
  {
    return;
  }

  view = (ArrayView*)malloc(sizeof(ArrayView));
  if (!view)
  {
    return;
  }
  view->data = nullptr;
  view->nTuples = 0;
  view->nComponents = 1;
  view->capacity = 0;
  view->elementSize = elementSize;
  view->elementType = elementType;
  view->updateCallback = nullptr;
  view->userData = &view->elementType;
  view->clientData = nullptr;
  view->referenceCount = 1;
}

void ArrayReferenceCounterGet(ArrayView* view, int* referenceCount)
{
  *referenceCount = view->referenceCount;
}

void ArrayReferenceCounterSet(ArrayView* view, int count)
{
  view->referenceCount = count;
}

void ArrayIncrementReference(ArrayView* view)
{
  if (view)
  {
    view->referenceCount++;
  }
}

void ArrayDecrementReference(ArrayView* view)
{
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

void ArrayDelete(ArrayView* view)
{
  if (view)
  {
    ArrayDecrementReference(view);
  }
}

void ArrayDataPointerSet(ArrayView* view, void* data)
{
  view->data = data;
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void ArrayNumberOfTuplesSet(ArrayView* view, int nTuples)
{
  if (nTuples > view->capacity)
  {
    ArrayResize(view, nTuples);
  }
  view->nTuples = nTuples;
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void ArrayNumberOfComponentsSet(ArrayView* view, int nComponents)
{
  view->nComponents = nComponents;
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void ArrayDataPointerGet(ArrayView* view, void** data)
{
  *data = view->data;
}

void ArrayNumberOfTuplesGet(ArrayView* view, int* nTuples)
{
  *nTuples = view->nTuples;
}

void ArrayNumberOfComponentsGet(ArrayView* view, int* nComponents)
{
  *nComponents = view->nComponents;
}

void ArrayResize(ArrayView* view, int newCapacity)
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
  else
  {
    void* newData = realloc(view->data, newCapacity * view->elementSize * view->nComponents);
    if (!newData)
    {
      printf("Memory allocation failed.\n");
    }
    view->data = newData;
    view->capacity = newCapacity;
  }
  if (view->updateCallback)
  {
    view->updateCallback(view, view->userData);
  }
}

void ArrayElementTypeGet(ArrayView* view, int* elementType)
{
  *elementType = view->elementType;
}

void ArrayShallowCopy(ArrayView* dest, const ArrayView* src)
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

void ArrayUpdateCallbackSet(ArrayView* view, ArrayUpdateCallback callback, void* clientData)
{
  if (!view)
    return;
  view->updateCallback = callback;
  view->clientData = clientData;
}
