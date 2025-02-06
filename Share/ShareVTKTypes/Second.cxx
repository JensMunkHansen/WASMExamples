#include "Second.h"
#include <cstdio>
#include <vtkPolyData.h>

#include <emscripten/bind.h>
#include <iostream>

void SecondModule::Consume(vtkPolyData* polyData)
{
  printf("Object name: %s\n", polyData->GetObjectName().c_str());
}

EMSCRIPTEN_BINDINGS(vtk_bindings)
{
  emscripten::function(
    "ConsumePolyData",
    +[](uintptr_t ptr) { SecondModule::Consume(reinterpret_cast<vtkPolyData*>(ptr)); });
}
