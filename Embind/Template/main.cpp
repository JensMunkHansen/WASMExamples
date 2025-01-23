#include "emscripten/bind.h"
#include <iostream>
#include <string>
#include <vector>

void process(std::string&& str)
{
  std::cout << "Processing: " << str << std::endl;
}

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

    std::string s = "Hello, world!";

    // Incorrect use of std::forward: s is not an rvalue, so this is UB
    process(std::forward<std::string>(s));

#ifdef __SANITIZE_ADDRESS__
    int* ptr = new int(42); // Dynamically allocate memory.
    delete ptr;             // Free the memory.
    // Use after free: Access the pointer after deleting it.
    std::cout << "Use after free value: " << *ptr << std::endl;
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
