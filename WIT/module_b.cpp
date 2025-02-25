// module_b.cpp
#include "shared.wit.h"
#include "wit-bindgen/cpp-wasi/wasi.h"
#include <iostream>

// Implementation of the ModuleB interface
extern "C" void shared_module_b_process_person(shared::Person person)
{
  std::cout << "Processing Person: " << person.name << " (" << person.age << " years old)"
            << std::endl;
}
