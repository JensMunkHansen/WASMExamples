#pragma once
#include "factory_and_base.h" // Base class

// Derived class
class Derived : public Base
{
public:
  void doSomething() const override;
};
