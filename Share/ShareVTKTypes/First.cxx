#include "First.h"

#include <vtkObject.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include <emscripten/bind.h>
#include <iostream>

vtkPolyData* FirstModule::CreateSphere()
{
  vtkNew<vtkSphereSource> sphere;
  sphere->SetPhiResolution(10);
  sphere->SetThetaResolution(10);
  sphere->Update();

  vtkSmartPointer<vtkPolyData> polyDataCopy = vtkSmartPointer<vtkPolyData>::New();
  polyDataCopy->ShallowCopy(sphere->GetOutput());

  // Increase the reference count so the caller owns it
  polyDataCopy->Register(nullptr);

  return polyDataCopy.GetPointer();
}

void FirstModule::DestroyPolyData(vtkPolyData* polyData)
{
  if (polyData)
  {
    std::cout << "Number of points: " << polyData->GetNumberOfPoints() << std::endl;
    std::cout << "Number of references: " << polyData->GetReferenceCount() << std::endl;
    polyData->UnRegister(nullptr);
  }
}

void FirstModule::Consume(vtkPolyData* polyData)
{
  if (polyData)
  {
    // Crashes!!! (VTABLE)
    printf("Class name: %s\n", polyData->GetClassName());
  }
}

EMSCRIPTEN_BINDINGS(vtk_bindings)
{
  emscripten::function(
    "CreateSphere",
    +[]() -> uintptr_t { return reinterpret_cast<uintptr_t>(FirstModule::CreateSphere()); });

  emscripten::function(
    "DestroyPolyData",
    +[](uintptr_t ptr) { FirstModule::DestroyPolyData(reinterpret_cast<vtkPolyData*>(ptr)); });
  emscripten::function(
    "ConsumePolyData",
    +[](uintptr_t ptr) { FirstModule::Consume(reinterpret_cast<vtkPolyData*>(ptr)); });
}
