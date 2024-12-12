#pragma once

#include <memory>
#include <string>

// Base class
class Base
{
public:
  virtual ~Base() = default;
  virtual void doSomething() const = 0;
};

// Factory class
class Factory
{
public:
  static std::unique_ptr<Base> create(const std::string& type);
};
