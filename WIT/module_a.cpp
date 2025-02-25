// module_a.cpp
#include "shared.wit.h"
#include "wit-bindgen/cpp-wasi/wasi.h"
#include <cstring>
#include <memory>
#include <string>

// Implementation of the ModuleA interface
extern "C" shared::Person shared_module_a_create_person(const char* name, uint32_t age)
{
  shared::Person person;
  person.name = name;
  person.age = age;
  return person;
}
