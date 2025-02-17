#pragma once

#include "array_abi.h"

#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <utility>

namespace arrays
{

template <typename ScalarType>
struct scalarTypeTraits;

#define DEFINE_ARRAY_TYPE_TRAITS(TYPE, ID)                                                         \
  template <>                                                                                      \
  struct scalarTypeTraits<TYPE>                                                                    \
  {                                                                                                \
    static constexpr int VTKTypeID() { return ID; }                                                \
  };

DEFINE_ARRAY_TYPE_TRAITS(float, 10)
DEFINE_ARRAY_TYPE_TRAITS(double, 11)
DEFINE_ARRAY_TYPE_TRAITS(int, 6)
DEFINE_ARRAY_TYPE_TRAITS(unsigned int, 7)
DEFINE_ARRAY_TYPE_TRAITS(short, 4)
DEFINE_ARRAY_TYPE_TRAITS(unsigned short, 5)
DEFINE_ARRAY_TYPE_TRAITS(char, 2)
DEFINE_ARRAY_TYPE_TRAITS(unsigned char, 3)
DEFINE_ARRAY_TYPE_TRAITS(long long, 12)
DEFINE_ARRAY_TYPE_TRAITS(unsigned long, 13)

#undef DEFINE_ARRAY_TYPE_TRAITS

// Iterator class for tuples
template <typename T, int N>
class TupleIterator
{
public:
  using ValueType = std::array<T, N>;
  using Pointer = ValueType*;
  using Reference = ValueType&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  TupleIterator(T* data)
    : data_(data)
  {
  }

  template <std::size_t... I>
  ValueType makeTuple(std::index_sequence<I...>) const
  {
    return { data_[I]... };
  }

  ValueType operator*() const { return makeTuple(std::make_index_sequence<N>{}); }

  TupleIterator& operator++()
  {
    data_ += N;
    return *this;
  }
  TupleIterator operator++(int)
  {
    TupleIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  bool operator==(const TupleIterator& other) const { return data_ == other.data_; }
  bool operator!=(const TupleIterator& other) const { return !(*this == other); }

private:
  T* data_;
};

// Compile-time fixed-size array wrapper
template <typename T, int N>
class ArrayWrapper
{
public:
  using ValueType = T;
  using Iterator = TupleIterator<T, N>;
  using ConstIterator = TupleIterator<const T, N>;
  static constexpr int ElementType = scalarTypeTraits<T>::VTKTypeID();

  ArrayWrapper(int numTuples)
    : numTuples_(numTuples)
  {
    ArrayCreate(view_, ElementType);
    assert(view_);
    // We allocate
    T* pMemory = static_cast<T*>(malloc(numTuples * N * sizeof(T)));
    ArrayDataPointerSet(view_, pMemory);
    ArrayNumberOfTuplesSet(view_, numTuples);
    ArrayNumberOfComponentsSet(view_, N); // Could be part of macro
    resize(numTuples);
  }

  ~ArrayWrapper() { ArrayDelete(view_); }

  void resize(int newTuples)
  {
    ArrayResize(view_, newTuples);
    syncFromABI();
  }

  int getNumTuples() const
  {
    int nTuples;
    ArrayNumberOfTuplesGet(view_, &nTuples);
    return nTuples;
  }

  ValueType* data() { return data_; }

  void syncFromABI()
  {
    void* ptr;
    ArrayDataPointerGet(view_, &ptr);
    data_ = static_cast<ValueType*>(ptr);
  }

  Iterator begin() { return Iterator(data_); }
  Iterator end() { return Iterator(data_ + getNumTuples() * N); }
  ConstIterator begin() const { return ConstIterator(data_); }
  ConstIterator end() const { return ConstIterator(data_ + getNumTuples() * N); }

private:
  ArrayView* view_ = nullptr;
  ValueType* data_ = nullptr;
  int numTuples_;
};

} // namespace arrays
