#pragma once
#include "sequences/sequence.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/bit_sequence.hpp"
#include "exceptions.hpp"

namespace myLib {

template<typename T, typename T2>
Sequence<T2>* Map(Sequence<T>* seq, T2 (*func)(T)) {
    if (!seq) {
        throw EmptyCollectionException("Sequence пуста");
    }

    size_t len = seq->GetLength();
    auto* result = new ArraySequence<T2>(); 
    
    for (size_t i = 0; i < len; i++) {
        result->Append(func(seq->Get(i)));
    }
    return result;
}

template<typename T>
Sequence<T>* Where(Sequence<T>* seq, bool (*predicate)(T)) {
    if (!seq) {
        throw EmptyCollectionException("Sequence пуста");
    }

    size_t len = seq->GetLength();
    auto* result = new ArraySequence<T>();

    for (size_t i = 0; i < len; i++) {
        T item = seq->Get(i);
        if (predicate(item)) {
            result->Append(item);
        }
    }
    return result;
}

template<typename T, typename T2>
Sequence<T2>* Reduce(Sequence<T>* seq, T2 (*func)(T2, T), T2 t) {
    if (!seq) {
        throw EmptyCollectionException("Sequence пуста");
    }

    size_t len = seq->GetLength();

    for (size_t i = 0; i < len; i++) {
        t = func(t, seq->Get(i));
    }

    auto* result = new ArraySequence<T2>();
    result->Append(t);
    return result;
}

}