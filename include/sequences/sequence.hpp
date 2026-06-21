#pragma once

#include "exceptions.hpp"
#include "sequence_iterator.hpp"

namespace myLib {

template <class T>
class Sequence {
public:    
    virtual ~Sequence() = default;

    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(size_t index) = 0;
    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) = 0;
    virtual size_t GetLength() = 0;
    
    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, size_t index) = 0;
    virtual Sequence<T>* DeleteAt(size_t index) = 0;
    virtual Sequence<T>* Concat(Sequence<T> *list) = 0;
    virtual Sequence<T>* Set(size_t index, T value) = 0;

    virtual Sequence<T>* Empty() = 0;

    virtual std::string ToString() = 0;
    virtual std::string ToString(T elem) = 0;
    virtual T FromString(const std::string& s) = 0;

    virtual SequenceIterator<T> begin() = 0;
    virtual SequenceIterator<T> end() = 0;
};

}