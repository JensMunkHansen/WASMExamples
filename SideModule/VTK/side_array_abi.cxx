#include "side_array_abi.h"
#include "array_abi.h"
#include <iostream>

void Process(struct ArrayView* arrayView)
{
  // Simulate updates to the ArrayView to trigger the callback
  int nComponents;
  int nTuples;

  // Change the dimensions
  ArrayNumberOfTuplesSet(arrayView, 20);

  // Get number of dimensions
  ArrayNumberOfComponentsGet(arrayView, &nComponents);
  ArrayNumberOfTuplesGet(arrayView, &nTuples);

  // TODO: Wrap in C++ (templates)
  void* pData = nullptr;
  ArrayDataPointerGet(arrayView, &pData);
  if (pData)
  {
    float* pFloatData = (float*)pData;
    // Update vtkFloatArray values (for test purposes, we just set all values to 42.0)
    for (int i = 0; i < nTuples; ++i)
    {
      for (int j = 0; j < nComponents; ++j)
      {
        pFloatData[i * nComponents + j] = (float)i;
      }
    }
  }
}
