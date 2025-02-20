// Empty file

#include "array_abi.h"
#include "side_array_abi.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif
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
void SynchronizeHelper(ArrayView* view)
{
  std::cout << "Callback invoked! Updating array of type " << typeid(ScalarType).name() << "..."
            << std::endl;

  // Get the new dimensions
  int nTuples;
  int nComponents;
  ArrayNumberOfTuplesGet(view, &nTuples);
  ArrayNumberOfComponentsGet(view, &nComponents);

  if ((myArray->GetNumberOfTuples() != nTuples) ||
    (myArray->GetNumberOfComponents() != nComponents))
  {
    // Memory resized - set the pointer
    void* newDataPointer;
    ArrayDataPointerGet(view, &newDataPointer);
    size_t newLength = nTuples * nComponents;

    // Warning: This is incorrect:
    //
    // Correct ways:
    // - Pass instead ownership back-and-forth and also listen to
    // Modified() event.
    //
    // - Extend vtkCommonDataModel with our mesh type. Note parallel for
    //   and Dispatchers are exposed in vtkCommonCore
    myArray->Initialize();
    myArray->SetArray(static_cast<float*>(newDataPointer), newLength, 1);
    myArray->Modified();
  }
}

void Synchronize(ArrayView* view, void* userData)
{
  std::cout << "Synchronize" << std::endl;

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
#ifdef __EMSCRIPTEN__
void TestMe()
#else
int main(int argc, char* argv[])
#endif
{
  std::cout << "SetNumberOfTuples" << std::endl;
  myArray->SetNumberOfTuples(10);
  std::cout << "SetNumberOfComponents" << std::endl;
  myArray->SetNumberOfComponents(1);
  myArray->Allocate(10);
  // Just a test to set something by main module
  myArray->SetValue(0, 1.0f);

  ArrayView* arrayView = nullptr;
  std::cout << "ArrayCreate" << std::endl;
  ArrayCreate(arrayView, VTK_FLOAT);

  if (arrayView == nullptr)
  {
    std::cout << "arrayView is nullptr" << std::endl;
  }

  std::cout << "ArrayNumberOfTuplesSet" << std::endl;
  ArrayNumberOfTuplesSet(arrayView, myArray->GetNumberOfTuples());
  std::cout << "ArrayNumberOfComponentsSet" << std::endl;
  ArrayNumberOfComponentsSet(arrayView, myArray->GetNumberOfComponents());
  std::cout << "ArrayDataPointerSet" << std::endl;
  ArrayDataPointerSet(arrayView, myArray->GetVoidPointer(0));
  // Set the callback for this vector
  void* clientData = nullptr;
  std::cout << "ArrayUpdateCallbackSet" << std::endl;
  ArrayUpdateCallbackSet(arrayView, Synchronize, clientData);

  std::cout << "Process" << std::endl;

  // Do some processing in another WASM module (not knowing VTK)
  Process(arrayView);

  // Show results, memory resized and updated
  int nTuples;
  ArrayNumberOfTuplesGet(arrayView, &nTuples);
  for (int i = 0; i < nTuples; i++)
  {
    std::cout << i << ": " << myArray->GetValue(i) << std::endl;
  }
  // Cleanup
  ArrayDelete(arrayView);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(SideModuleTest)
{
  emscripten::function("TestMe", &TestMe);
}
#endif
