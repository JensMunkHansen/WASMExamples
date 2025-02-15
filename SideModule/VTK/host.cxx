// Empty file

#include "side_abi.h"
#include "vector_abi.h"
#include <emscripten/bind.h>
#include <iostream>
#include <vtkFloatArray.h>
#include <vtkNew.h>

// Just a static array for test purpose
vtkNew<vtkFloatArray> myArray;

// Callback function to be called when VectorView is updated
void MyUpdateCallback(VectorView* view, void* userData)
{
  int nTuples = GetNumberOfTuples(view);
  int nComponents = GetNumberOfComponents(view);

  std::cout << "Callback invoked! Updating vtkFloatArray..." << std::endl;
  std::cout << "New size: " << nTuples << " tuples, " << nComponents << " components" << std::endl;

  // Resize the vtkFloatArray to match the updated VectorView dimensions
  myArray->SetNumberOfComponents(nComponents);
  myArray->SetNumberOfTuples(nTuples);

  // Update vtkFloatArray values (for test purposes, we just set all values to 42.0)
  for (int i = 0; i < nTuples; ++i)
  {
    for (int j = 0; j < nComponents; ++j)
    {
      myArray->SetComponent(i, j, 42.0f);
    }
  }
}

void TestMe()
{
  myArray->SetNumberOfTuples(10);
  myArray->SetNumberOfComponents(3);
  myArray->SetValue(0, 1.0f);
  VectorView* vectorView = CreateVector(VTK_INT);
  SetNumberOfTuples(vectorView, 10);
  SetNumberOfComponents(vectorView, 3);

  // Set the callback for this vector
  SetUpdateCallback(vectorView, MyUpdateCallback, nullptr);

  Process(vectorView);
}

EMSCRIPTEN_BINDINGS(SideModuleTest)
{
  emscripten::function("TestMe", &TestMe);
}
