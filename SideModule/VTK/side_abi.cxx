#include "side_abi.h"
#include <iostream>

void Process(struct VectorView* vectorView)
{
  // Simulate updates to the VectorView to trigger the callback
  std::cout << "Setting number of tuples to 15..." << std::endl;
  SetNumberOfTuples(vectorView, 15);

  std::cout << "Resizing vector to 20 tuples..." << std::endl;
  ResizeVector(vectorView, 20);

  // Cleanup
  DestroyVector(vectorView);
}
