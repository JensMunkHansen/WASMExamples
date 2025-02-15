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
  int nTuples;
  GetNumberOfTuples(view, &nTuples);
  int nComponents;
  GetNumberOfComponents(view, &nComponents);

  std::cout << "Callback invoked! Updating vtkFloatArray..." << std::endl;
  std::cout << "New size: " << nTuples << " tuples, " << nComponents << " components" << std::endl;

  if ((myArray->GetNumberOfTuples() != nTuples) ||
    (myArray->GetNumberOfComponents() != nComponents))
  {
    // We need to set the data pointer
    void* newDataPointer;
    GetDataPointer(view, &newDataPointer);
    size_t newLength = nTuples * nComponents * sizeof(float); // HACK
    myArray->SetVoidArray(newDataPointer, newLength, 1, VTK_FLOAT);
  }
  // Resize the vtkFloatArray to match the updated VectorView dimensions
  myArray->SetNumberOfComponents(nComponents);
  myArray->SetNumberOfTuples(nTuples);

  // TODO: Set pointer if reallocated
}

void TestMe()
{
  myArray->SetNumberOfTuples(10);
  myArray->SetNumberOfComponents(3);
  myArray->SetValue(0, 1.0f);
  VectorView* vectorView;
  CreateVector(VTK_INT, &vectorView);
  SetNumberOfTuples(vectorView, 10);
  SetNumberOfComponents(vectorView, 3);
  SetDataPointer(vectorView, myArray->GetVoidPointer(0));

  // Set the callback for this vector
  SetUpdateCallback(vectorView, MyUpdateCallback, nullptr);

  Process(vectorView);

  for (int i = 0; i < 20; i++)
  {
    std::cout << myArray->GetValue(i) << std::endl;
  }
}

EMSCRIPTEN_BINDINGS(SideModuleTest)
{
  emscripten::function("TestMe", &TestMe);
}
