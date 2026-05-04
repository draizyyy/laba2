#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"
#include "core/linked_list.hpp"

namespace myLib {

template <typename T>
class BitProxy {
private:
    T& num;
    int bitIndex;

public:
    BitProxy(T& value, int index) : num(value), bitIndex(index) {}
    
    BitProxy& operator=(bool value) {
        if (value) {
            num |= (static_cast<T>(1) << bitIndex);
        } else {
            num &= ~(static_cast<T>(1) << bitIndex);
        }
        return *this;
    }
    
    operator bool() {
        return (num >> bitIndex) & static_cast<T>(1);
    }
};

template <typename T>
class Bit {
private:
    T value;

public:
    Bit() : value(0) {}
    Bit(T val) : value(val) {}
    Bit(const Bit& other) : value(other.value) {}
    
    Bit& operator=(Bit& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }
    
    static int BitCount() {
        return sizeof(T) * 8;
    }
    
    bool operator[](int index) const {
        if (index < 0 || index >= sizeof(T) * 8) {
            throw IndexOutOfRangeException();
        }
        return (value >> index) & static_cast<T>(1);
    }
    
    BitProxy<T> operator[](int index) {
        if (index < 0 || index >= sizeof(T) * 8) {
            throw IndexOutOfRangeException();
        }
        return BitProxy<T>(value, index);
    }
    
    T GetValue() {
        return value;
    }
    
    void SetValue(T val) {
        value = val;
    }
    
    operator bool() {
        return value != 0;
    }
    
    Bit operator&(const Bit& other) {
        return Bit(value & other.value);
    }
    
    Bit operator|(const Bit& other) {
        return Bit(value | other.value);
    }
    
    Bit operator^(const Bit& other) {
        return Bit(value ^ other.value);
    }
    
    Bit operator~() {
        return Bit(~value);
    }
    
    bool operator==(const Bit& other) {
        return value == other.value;
    }
    
    bool operator!=(const Bit& other) {
        return value != other.value;
    }
};

template <typename T>
class BitSequence : public Sequence<Bit<T>> {
private:
    DynamicArray<Bit<T>>* data;

public:
    BitSequence() : data(new DynamicArray<Bit<T>>(0)) {}
    
    BitSequence(Bit<T>* items, int count) : data(new DynamicArray<Bit<T>>(items, count)) {}
    
    BitSequence(const BitSequence& other) : data(new DynamicArray<Bit<T>>(0)) {
        data->Resize(other.data->GetSize());
        for (int i = 0; i < other.data->GetSize(); i++) {
            data->Set(i, other.data->Get(i));
        }
    }
    
    ~BitSequence() override {
        delete data;
    }
    
    BitSequence& operator=(const BitSequence& other) {
        if (this != &other) {
            delete data;
            data = new DynamicArray<Bit<T>>(0);
            data->Resize(other.data->GetSize());
            for (int i = 0; i < other.data->GetSize(); i++) {
                data->Set(i, other.data->Get(i));
            }
        }
        return *this;
    }
    
    Bit<T> GetFirst() override {
        if (data->GetSize() == 0) {
            throw IndexOutOfRangeException();
        }
        return data->Get(0);
    }
    
    Bit<T> GetLast() override {
        if (data->GetSize() == 0) {
            throw IndexOutOfRangeException();
        }
        return data->Get(data->GetSize() - 1);
    }
    
    Bit<T> Get(int index) override {
        if (index < 0 || index >= data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        return data->Get(index);
    }
    
    int GetLength() override {
        return data->GetSize();
    }
    
    Bit<T> operator[](int index) { 
        if (index < 0 || index >= data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        return data->Get(index); 
    }
    
    Sequence<Bit<T>>* GetSubsequence(int startIndex, int endIndex) override {
        if (startIndex < 0 || endIndex >= data->GetSize() || startIndex > endIndex) {
            throw IndexOutOfRangeException();
        }
        
        int len = endIndex - startIndex + 1;
        Bit<T>* items = new Bit<T>[len];
        
        for (int i = 0; i < len; i++) {
            items[i] = data->Get(startIndex + i);
        }
        
        Sequence<Bit<T>>* result = new BitSequence<T>(items, len);
        delete[] items;
        return result;
    }
    
    Sequence<Bit<T>>* Append(Bit<T> item) override {
        data->Resize(data->GetSize() + 1);
        data->Set(data->GetSize() - 1, item);
        return this;
    }
    
    Sequence<Bit<T>>* Prepend(Bit<T> item) override {
        int oldSize = data->GetSize();
        data->Resize(oldSize + 1);
        for (int i = oldSize; i > 0; i--) {
            data->Set(i, data->Get(i - 1));
        }
        data->Set(0, item);
        return this;
    }
    
    Sequence<Bit<T>>* InsertAt(Bit<T> item, int index) override {
        if (index < 0 || index > data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        int oldSize = data->GetSize();
        data->Resize(oldSize + 1);
        for (int i = oldSize; i > index; i--) {
            data->Set(i, data->Get(i - 1));
        }
        data->Set(index, item);
        return this;
    }
    
    Sequence<Bit<T>>* Concat(Sequence<Bit<T>>* list) override {
        int len = data->GetSize() + list->GetLength();
        Bit<T>* t = new Bit<T>[len];
        
        for (int i = 0; i < data->GetSize(); i++) {
            t[i] = data->Get(i);
        }
        for (int i = 0; i < list->GetLength(); i++) {
            t[data->GetSize() + i] = list->Get(i);
        }

        Sequence<Bit<T>>* seq = new BitSequence<T>(t, len);
        delete[] t;
        return seq;
    }
    
    IEnumerator<Bit<T>>* GetEnumerator() override {
        return new ArrayEnumerator<Bit<T>>(data);
    }
    
    template <typename T2>
    BitSequence<T2>* Map(Bit<T2> (*func)(Bit<T>)) {
        BitSequence<T2>* t = new BitSequence<T2>();
        t->data->Resize(data->GetSize());
        for (int i = 0; i < data->GetSize(); i++) {
            t->data->Set(i, func(data->Get(i)));
        }
        return t;
    }
    
    BitSequence<T>* Where(bool (*predicate)(Bit<T>)) {
        BitSequence<T>* t = new BitSequence<T>();
        for (int i = 0; i < data->GetSize(); i++) {
            if (predicate(data->Get(i))) {
                t->Append(data->Get(i));
            }
        }
        return t;
    }
    
    template<typename T2>
    T2 Reduce(T2 (*func)(T2, Bit<T>), T2 t2) {
        T2 t = t2;
        for (int i = 0; i < data->GetSize(); i++) {
            t = func(t, data->Get(i));
        }
        return t;
    }
    
    BitSequence<T> operator&(const BitSequence<T>& other) {
        if (data->GetSize() != other.data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        BitSequence<T> t;
        t.data->Resize(data->GetSize());
        for (int i = 0; i < data->GetSize(); i++) {
            t.data->Set(i, data->Get(i) & other.data->Get(i));
        }
        return t;
    }
    
    BitSequence<T> operator|(const BitSequence<T>& other) {
        if (data->GetSize() != other.data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        BitSequence<T> t;
        t.data->Resize(data->GetSize());
        for (int i = 0; i < data->GetSize(); i++) {
            t.data->Set(i, data->Get(i) | other.data->Get(i));
        }
        return t;
    }
    
    BitSequence<T> operator^(const BitSequence<T>& other) {
        if (data->GetSize() != other.data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        BitSequence<T> t;
        t.data->Resize(data->GetSize());
        for (int i = 0; i < data->GetSize(); i++) {
            t.data->Set(i, data->Get(i) ^ other.data->Get(i));
        }
        return t;
    }
    
    BitSequence<T> operator~() {
        BitSequence<T> t;
        t.data->Resize(data->GetSize());
        for (int i = 0; i < data->GetSize(); i++) {
            t.data->Set(i, ~data->Get(i));
        }
        return t;
    }
};

}