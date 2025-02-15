// Empty file

#include "side_abi.h"
#include "vector_abi.h"
#include <emscripten/bind.h>
#include <iostream>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkNew.h>

#include <vtkType.h>
#include <vtkTypeTraits.h>

// Just a static array for test purposes
vtkNew<vtkFloatArray> myArray;

template <typename VTKArrayType, typename ScalarType>
void SynchronizeHelper(VectorView* view)
{
  std::cout << "Callback invoked! Updating array of type " << typeid(ScalarType).name() << "..."
            << std::endl;

  // Get the new dimensions
  int nTuples;
  int nComponents;
  GetNumberOfTuples(view, &nTuples);
  GetNumberOfComponents(view, &nComponents);

  if ((myArray->GetNumberOfTuples() != nTuples) ||
    (myArray->GetNumberOfComponents() != nComponents))
  {
    // Memory resized - set the pointer
    void* newDataPointer;
    GetDataPointer(view, &newDataPointer);
    size_t newLength = nTuples * nComponents;
    // Does not change reference count
    myArray->SetVoidArray(newDataPointer, newLength, 1, vtkTypeTraits<ScalarType>::VTKTypeID());
  }
  // Resize the vtkFloatArray to match the updated VectorView dimensions
  myArray->SetNumberOfComponents(nComponents);
  myArray->SetNumberOfTuples(nTuples);
}

void Synchronize(VectorView* view, void* userData)
{
  int dataType = *reinterpret_cast<int*>(userData);
  switch (dataType)
  {
    case VTK_FLOAT:
      SynchronizeHelper<vtkFloatArray, float>(view);
      break;
    case VTK_DOUBLE:
      SynchronizeHelper<vtkDoubleArray, double>(view);
      break;
    case VTK_INT:
      SynchronizeHelper<vtkIntArray, int>(view);
      break;
    // Add more cases as needed for other VTK types
    default:
      std::cerr << "Unsupported data type in Synchronize callback!" << std::endl;
      break;
  }
}

static int MyArrayType = VTK_FLOAT;
void TestMe()
{
  myArray->SetNumberOfTuples(10);
  myArray->SetNumberOfComponents(1);
  // Just a test to set something by main module
  myArray->SetValue(0, 1.0f);

  VectorView* vectorView = nullptr;
  CreateVector(vectorView, VTK_FLOAT);
  SetNumberOfTuples(vectorView, myArray->GetNumberOfTuples());
  SetNumberOfComponents(vectorView, myArray->GetNumberOfComponents());
  SetDataPointer(vectorView, myArray->GetVoidPointer(0));
  // Set the callback for this vector
  SetUpdateCallback(vectorView, Synchronize, &MyArrayType);

  // Do some processing in another WASM module (not knowing VTK)
  Process(vectorView);
  int nTuples;
  GetNumberOfTuples(vectorView, &nTuples);
  for (int i = 0; i < nTuples; i++)
  {
    std::cout << i << ": " << myArray->GetValue(i) << std::endl;
  }
  // Cleanup
  DeleteVector(vectorView);
}

EMSCRIPTEN_BINDINGS(SideModuleTest)
{
  emscripten::function("TestMe", &TestMe);
}
