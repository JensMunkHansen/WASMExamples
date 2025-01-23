#include "emscripten/bind.h"
#include <iostream>
#include <string>
#include <vector>

template <typename FloatType>
class TemplateClass
{
public:
  TemplateClass()
    : Data(0)
    , VectorData(1)
  {
  }
  FloatType Data;
  std::vector<FloatType> VectorData;
  void setData(FloatType x)
  {
    Data = x;

#ifdef __SANITIZE_ADDRESS__
    int* ptr = new int(42); // Dynamically allocate memory.
    delete ptr;             // Free the memory.
    // Use after free: Access the pointer after deleting it.
    std::cout << "Use after free value: " << *ptr << std::endl;
#endif

#ifdef __SANITIZE_UNDEFINED__
    int max = std::numeric_limits<int>::max();
    int result = max + 1; // Signed integer overflow: Undefined Behavior
#endif
  }
  FloatType getData() const
  {
    return Data;
  }
};

EMSCRIPTEN_BINDINGS(TemplateClass)
{

  // some other bindings here

  emscripten::register_vector<float>("VectorFloat");

  emscripten::class_<TemplateClass<float>>("TemplateClassFloat")
    .constructor<>()
    .property("Data", &TemplateClass<float>::getData, &TemplateClass<float>::setData);
  //    .property("VectorData",
}
