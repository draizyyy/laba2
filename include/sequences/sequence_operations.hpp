#pragma once

#include "sequence_iterator.hpp"
#include "sequence.hpp"

namespace myLib {

template<typename T>
Sequence<T>* Map(Sequence<T>* seq, T (*func)(T)) {
    if (!seq) {
        return nullptr;
    }
    Sequence<T>* result = seq->Empty();

    for (const auto& el : *seq) {
        result->Append(func(el));
    }

    return result;
}

template<typename T>
Sequence<T>* Where(Sequence<T>* seq, bool (*predicate)(T)) {
    if (!seq) {
        return nullptr;
    }
    Sequence<T>* result = seq->Empty();

    for (const auto& el : *seq) {
        if (predicate(el)) {
            result->Append(el);
        }
    }

    return result;
}

template<typename T>
T Reduce(Sequence<T>* seq, T (*func)(T, T), T startValue) {
    if (!seq) {
        return startValue;
    }
    T result = startValue;

    for (const auto& el : *seq) {
        result = func(result, el);
    }

    return result;
}

}