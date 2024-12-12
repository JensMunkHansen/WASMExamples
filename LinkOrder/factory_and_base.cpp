#include "factory_and_base.h"
#include "derived.h" // Include the definition of Derived
#include <iostream>
#include <stdexcept>

// Factory implementation
std::unique_ptr<Base> Factory::create(const std::string& type)
{
  if (type == "Derived")
  {
    return std::make_unique<Derived>(); // Now works because Derived is complete
  }
  throw std::invalid_argument("Unknown type: " + type);
}
