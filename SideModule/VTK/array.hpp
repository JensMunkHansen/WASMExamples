#pragma once
#include "array_abi.h"
#include <array>
#include <cassert>
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

  TupleIterator(T* data, int numComponents)
    : data_(data)
    , numComponents_(numComponents)
  {
  }

  // Compile-time tuple maker
  template <std::size_t... I>
  ValueType makeTuple(std::index_sequence<I...>) const
  {
    return { data_[I]... };
  }

  ValueType operator*() const { return makeTuple(std::make_index_sequence<N>{}); }

  TupleIterator& operator++()
  {
    data_ += numComponents_;
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
  int numComponents_;
};

template <typename T>
class ArrayWrapper
{
public:
  using ValueType = T;
  static constexpr int ElementType = scalarTypeTraits<T>::VTKTypeID();

  ArrayWrapper(int numTuples, int numComponents)
    : numComponents_(numComponents)
  {
    ArrayCreate(view_, ElementType);
    assert(view_);
    ArrayNumberOfTuplesSet(view_, numTuples);
    ArrayNumberOfComponentsSet(view_, numComponents);
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

  int getNumComponents() const { return numComponents_; }

  ValueType* data() { return data_; }

  void syncFromABI()
  {
    void* ptr;
    ArrayDataPointerGet(view_, &ptr);
    data_ = static_cast<ValueType*>(ptr);
  }

  template <int N>
  using Iterator = TupleIterator<ValueType, N>;
  template <int N>
  using ConstIterator = TupleIterator<const ValueType, N>;

  template <int N>
  Iterator<N> begin()
  {
    return Iterator<N>(data_, numComponents_);
  }
  template <int N>
  Iterator<N> end()
  {
    return Iterator<N>(data_ + getNumTuples() * numComponents_, numComponents_);
  }
  template <int N>
  ConstIterator<N> begin() const
  {
    return ConstIterator<N>(data_, numComponents_);
  }
  template <int N>
  ConstIterator<N> end() const
  {
    return ConstIterator<N>(data_ + getNumTuples() * numComponents_, numComponents_);
  }

private:
  ArrayView* view_ = nullptr;
  ValueType* data_ = nullptr;
  int numComponents_;
};

} //
