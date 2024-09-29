#pragma once

#include "base.h"

class DerivedClass : public BaseClass
{
public:
  DerivedClass() {}
  ~DerivedClass() {}

  void greet() override;
};
