#pragma once
#include "iterators/ienumerator.hpp"
#include "iterators/ienumerable.hpp"

namespace myLib {

template <class T>
class Sequence : public IEnumerable<T> {
public:
    virtual ~Sequence() = default;

    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) = 0;
    virtual int GetLength() = 0;
    
    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T> *list) = 0;

    // virtual Sequence <T>* Map(T2 (*)(T)) = 0;
    // virtual Sequence <T>* Where(bool (*)(T)) = 0;

    // template <typename T2>
    // virtual Sequence <T>* Reduce(T2 (*)(T2,T)) = 0;
    // virtual Option<T> GetFirst(bool (*)(T) = 0) = 0;
    // virtual Option<T> GetLast(bool (*)(T) = 0) = 0;
};
}