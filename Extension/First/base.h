#pragma once

class BaseClass
{
public:
  BaseClass() {}
  virtual ~BaseClass() {}

  virtual void greet();
};

class OtherClass
{
public:
  OtherClass();
  float f;
};
