#include "array_abi.h"
#include <cstdlib>
#include <cstring>

#ifndef __EMSCRIPTEN__
#include <cstdio>
#endif

// We don't have the full runtime, so stdio support

struct ArrayView
{
  void* data;
  int nTuples;
  int nComponents;
  int capacity; // Number of tuples
  int elementSize;
  int elementType; // Conforms to VTK type values
  int referenceCount;
  ArrayUpdateCallback updateCallback;
  void* userData;
  void* clientData; // Currently unused
};

// Helper function to get the size of each element based on VTK elementType
void ArrayElementSizeGet(int elementType, int* size)
{
  switch (elementType)
  {
    case 10:
      *size = sizeof(float); // VTK_FLOAT
      break;
    case 11:
      *size = sizeof(double); // VTK_DOUBLE
      break;
    case 2:
      *size = sizeof(char); // VTK_CHAR
      break;
    case 3:
      *size = sizeof(unsigned char); // VTK_UNSIGNED_CHAR
      break;
    case 4:
      *size = sizeof(short); // VTK_SHORT
      break;
    case 5:
      *size = sizeof(unsigned short); // VTK_UNSIGNED_SHORT
      break;
    case 6:
      *size = sizeof(int); // VTK_INT
      break;
    case 7:
      *size = sizeof(unsigned int); // VTK_UNSIGNED_INT
      break;
    case 12:
      *size = sizeof(long long); // VTK_LONG_LONG
      break;
    case 13:
      *size = sizeof(unsigned long long); // VTK_UNSIGNED_LONG_LONG
      break;
    default:
      *size = 0; // Unsupported type
  }
}

void ArrayCreate(ArrayView*& view, int elementType)
{
  int elementSize;
  ArrayElementSizeGet(elementType, &elementSize);
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

void ArrayReferenceCounterGet(const ArrayView* view, int* referenceCount)
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

void ArrayDecrementReference(ArrayView*& view)
{
  if (view)
  {
    view->referenceCount--;
    if (view->referenceCount == 0)
    {
      if (view->data)
      {
#ifndef __EMSCRIPTEN__
        printf("freeing content\n");
#endif
        free(view->data);
        view->data = nullptr;
      }
      free(view);
#ifndef __EMSCRIPTEN__
      printf("freeing\n");
#endif
      view = nullptr;
    }
  }
}

void ArrayDelete(ArrayView*& view)
{
  if (view)
  {
    ArrayDecrementReference(view);
  }
}

void ArrayDataPointerSet(ArrayView* view, void* data)
{
  if (view)
  {
    view->data = data;
    if (view->updateCallback)
    {
      view->updateCallback(view, view->userData);
    }
  }
}

void ArrayNumberOfTuplesSet(ArrayView* view, int nTuples)
{
  if (view)
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
}

void ArrayNumberOfComponentsSet(ArrayView* view, int nComponents)
{
  if (view)
  {
    view->nComponents = nComponents;
    if (view->updateCallback)
    {
      view->updateCallback(view, view->userData);
    }
  }
}

void ArrayDataPointerGet(const ArrayView* view, void** data)
{
  if (view)
  {
    *data = view->data;
  }
  else
  {
    *data = nullptr;
  }
}

void ArrayNumberOfTuplesGet(const ArrayView* view, int* nTuples)
{
  if (view)
  {
    *nTuples = view->nTuples;
  }
  else
  {
    *nTuples = -1;
  }
}

void ArrayNumberOfComponentsGet(const ArrayView* view, int* nComponents)
{
  if (view)
  {
    *nComponents = view->nComponents;
  }
}

void ArrayResize(ArrayView* view, int newCapacity)
{
  if (view)
  {
    if (view->elementSize == 0)
    {
      return;
    }

    if (newCapacity <= view->capacity)
    {
      return;
    }
    else
    {
      // Must be compatible with how VTK is compiled!!!
      // Must use the original pointer!!!!
      void* newData = malloc(newCapacity * view->elementSize * view->nComponents);
      if (!newData)
      {
        // printf("Memory allocation failed.\n"); // requires -sEXIT_RUNTIME=1 -sFULL_ES3=1
      }
      view->data = newData;
      view->capacity = newCapacity;
    }
    if (view->updateCallback)
    {
      view->updateCallback(view, view->userData);
    }
  }
}

void ArrayElementTypeGet(const ArrayView* view, int* elementType)
{
  if (view)
  {
    *elementType = view->elementType;
  }
}

void ArrayShallowCopy(const ArrayView* src, ArrayView* dest)
{
  if (!dest || !src)
  {
    // printf("ShallowCopy: Null vector passed.\n");
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
