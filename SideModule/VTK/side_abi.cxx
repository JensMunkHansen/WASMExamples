#include "side_abi.h"
#include "vector_abi.h"
#include <iostream>

void Process(struct VectorView* vectorView)
{
  // Simulate updates to the VectorView to trigger the callback
  int nComponents;
  int nTuples;
  // Change the dimensions
  SetNumberOfTuples(vectorView, 20);

  // Get number of dimensions
  GetNumberOfComponents(vectorView, &nComponents);
  GetNumberOfTuples(vectorView, &nTuples);

  // TODO: Wrap in C++ (templates)
  void* pData = nullptr;
  GetDataPointer(vectorView, &pData);
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
