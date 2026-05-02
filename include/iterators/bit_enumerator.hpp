#pragma once
#include "ienumerator.hpp"
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"
#include "sequences/bit_sequence.hpp"

namespace myLib {

template<typename T>
class BitSequenceEnumerator : public IEnumerator<Bit<T>> {
private:
    DynamicArray<Bit<T>>* data;
    int currentIndex;

public:
    explicit BitSequenceEnumerator(DynamicArray<Bit<T>>* arr) : data(arr), currentIndex(-1) {}

    ~BitSequenceEnumerator() override = default;

    bool MoveNext() override {
        if (currentIndex + 1 < data->GetSize()) {
            currentIndex++;
            return true;
        }
        return false; 
    }

    Bit<T> Current() override {
        if (currentIndex < 0 || currentIndex >= data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        return data->Get(currentIndex);
    }

    void Reset() override {
        currentIndex = -1;
    }
};

}