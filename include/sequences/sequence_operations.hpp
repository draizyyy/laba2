#pragma once

#include "sequence.hpp"

namespace myLib {

template<typename T>
Sequence<T>* Map(Sequence<T>* seq, T (*func)(T)) {
    Sequence<T>* result = seq->Empty();

    for (size_t i = 0; i < seq->GetLength(); i++) {
        result->Append(func(seq->Get(i)));
    }

    return result;
}

template<typename T>
Sequence<T>* Where(Sequence<T>* seq, bool (*predicate)(T)) {
    Sequence<T>* result = seq->Empty();

    for (size_t i = 0; i < seq->GetLength(); i++) {
        T value = seq->Get(i);

        if (predicate(value)) {
            result->Append(value);
        }
    }

    return result;
}

template<typename T>
T Reduce(Sequence<T>* seq, T (*func)(T, T), T startValue) {
    T result = startValue;

    for (size_t i = 0; i < seq->GetLength(); i++) {
        result = func(result, seq->Get(i));
    }

    return result;
}

}