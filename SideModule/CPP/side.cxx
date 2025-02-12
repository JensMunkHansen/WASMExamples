#include "side.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif
  int side(int a);

  struct MyStruct
  {
    float a;
  };

  void CreateMyStruct(struct MyStruct* s)
  {
    // I know that we leak and we cannot even see the memory
    struct MyStruct* newStruct = (struct MyStruct*)malloc(sizeof(struct MyStruct));
    newStruct->a = 2.0f;
    printf("Side module: %3.2f\n", newStruct->a);
  }

  float GetValue(struct MyStruct* s)
  {
    printf("Side module: GetValue() %3.2f\n", s->a);
    return s->a;
  }

  void SetValue(struct MyStruct* s)
  {
    s->a = 2.0;
  }
#ifdef __cplusplus
}
#endif

class ISomeClass
{
public:
  ~ISomeClass() = default;

  float a;
};

class SomeClass : public ISomeClass
{
private:
  struct MyStruct internals;

public:
  SomeClass() { this->a = 1.0f; }
  ~SomeClass() = default;
};

#ifdef __cplusplus
extern "C"
{
#endif

  int side(int a)
  {
    SomeClass c;
    return a + c.a;
  }
#ifdef __cplusplus
}
#endif
