#include "emscripten/bind.h"
#include <vector>

template <typename FloatType> class TemplateClass {
public:
  TemplateClass() : Data(0), VectorData(1) {
  }
  FloatType Data;
  std::vector<FloatType> VectorData;
  void setData(FloatType x) { Data = x;}
  FloatType getData() const {return Data;}
};

EMSCRIPTEN_BINDINGS(TemplateClass) {

  // some other bindings here
  
  emscripten::register_vector<float>("VectorFloat");
  
  emscripten::class_<TemplateClass<float>>("TemplateClassFloat")
    .constructor<>()
    .property("Data", &TemplateClass<float>::getData, &TemplateClass<float>::setData);
  //    .property("VectorData", 
}
