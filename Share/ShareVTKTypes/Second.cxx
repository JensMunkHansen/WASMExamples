#include "Second.h"
#include <cstdio>
#include <vtkPolyData.h>

#include <emscripten/bind.h>
#include <iostream>

void SecondModule::Consume(vtkPolyData* polyData)
{
  printf("Number of points: %d\n", polyData->GetNumberOfPoints());
}

EMSCRIPTEN_BINDINGS(vtk_bindings)
{
  emscripten::function(
    "ConsumePolyData",
    +[](uintptr_t ptr) { SecondModule::Consume(reinterpret_cast<vtkPolyData*>(ptr)); });
}
