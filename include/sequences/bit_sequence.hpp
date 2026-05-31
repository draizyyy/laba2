#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"
#include "core/linked_list.hpp"
#include <string>
#include "exceptions.hpp"

namespace myLib {

template <typename T>
class BitProxy {
private:
    T& num;
    size_t bitIndex;
public:
    BitProxy(T& value, size_t index) : num(value), bitIndex(index) {}
    
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
    T value{};

public:
    Bit() = default;
    Bit(T val) : value(val) {}
    Bit(const Bit& other) : value(other.value) {}
    
    Bit& operator=(const Bit& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }
        
    static size_t BitCount() {
        return sizeof(T) * 8;
    }
    
    bool operator[](size_t index) const {
        if (index >= BitCount()) {
            throw IndexOutOfRangeException(index, index-1);
        }
        return (value >> index) & static_cast<T>(1);
    }
    
    BitProxy<T> operator[](size_t index) {
        if (index >= BitCount()) {
            throw IndexOutOfRangeException(index, index-1);
        }
        return BitProxy<T>(value, index);
    }
    
    T GetValue() const {
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

template<typename T>
std::istream& operator>>(std::istream& is, Bit<T>& b) {
    T val;
    is >> val;
    b.SetValue(val);   
    return is;
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const Bit<T>& b) {
    os << b.GetValue();
    return os;
}

template <typename T> class BitSequence;

template <typename T>
class BitSequenceProxy {
private:
    BitSequence<T>* seq;
    size_t index;
    
public:
    BitSequenceProxy(BitSequence<T>* s, size_t Index) : seq(s), index(Index) {}

    BitSequenceProxy& operator=(bool value) {
        seq->SetBit(index, value);
        return *this;
    }

    BitSequenceProxy& operator=(int value) {
        seq->SetBit(index, static_cast<bool>(value));
        return *this;
    }

    operator bool() const {
        return seq->GetBit(index);
    }
};

template <typename T>
class BitSequence : public Sequence<Bit<T>> {
private:
    DynamicArray<Bit<T>>* data;
    friend class BitSequenceProxy<T>;

    void SetBit(size_t index, bool value) {
        size_t elemIndex = index / Bit<T>::BitCount();
        size_t offset = index % Bit<T>::BitCount();
        Bit<T> elem = data->Get(elemIndex);
        elem[offset] = value;
        data->Set(elemIndex, elem);
    }
    bool GetBit(size_t index) const {
        size_t elemIndex = index / Bit<T>::BitCount();
        size_t offset = index % Bit<T>::BitCount();
        return data->Get(elemIndex)[offset];
    }

public:
    BitSequence() : data(new DynamicArray<Bit<T>>()) {}
    
    BitSequence(Bit<T>* items, size_t count) : data(new DynamicArray<Bit<T>>(items, count)) {}

    BitSequence(const DynamicArray<Bit<T>>& arr) : data(new DynamicArray<Bit<T>>(arr)) {}
    
    BitSequence(const BitSequence& other) : data(new DynamicArray<Bit<T>>(*other.data)) {}
    
    ~BitSequence() override {
        delete data;
    }
    
    BitSequence& operator=(const BitSequence& other) {
        if (this != &other) {
            delete data;
            data = new DynamicArray<Bit<T>>(*other.data);
        }
        return *this;
    }
    
    Bit<T> GetFirst() override {
        if (data->GetSize() == 0) {
            throw EmptyCollectionException("BitSequence");
        }
        return data->Get(0);
    }
    
    Bit<T> GetLast() override {
        if (data->GetSize() == 0) {
            throw EmptyCollectionException("BitSequence");
        }
        return data->Get(data->GetSize() - 1);
    }
    
    Bit<T> Get(size_t index) override {
        if (index >= data->GetSize()) {
            throw IndexOutOfRangeException(index, data->GetSize());
        }
        return data->Get(index);
    }
    
    size_t GetLength() override {
        return data->GetSize();
    }

    BitSequenceProxy<T> operator[](size_t index) { 
        size_t totalBits = data->GetSize() * Bit<T>::BitCount();
        if (index >= totalBits) {
            throw IndexOutOfRangeException(index, totalBits);
        }
        return BitSequenceProxy<T>(this, index);
    }

    bool operator[](size_t index) const {
        size_t totalBits = data->GetSize() * Bit<T>::BitCount();
        if (index >= totalBits) {
            throw IndexOutOfRangeException(index, totalBits);
        }
        return GetBit(index);
    }

    Sequence<Bit<T>>* GetSubsequence(size_t startIndex, size_t endIndex) override {
        if (startIndex > endIndex) {
            throw IndexOutOfRangeException(startIndex, endIndex);
        }
        if (endIndex > data->GetSize()) {
            throw IndexOutOfRangeException(endIndex, data->GetSize());
        }
        if (startIndex == endIndex) {
            return new BitSequence<T>();
        }
        
        DynamicArray<Bit<T>>* sub = data->SubArray(startIndex, endIndex + 1);
        Sequence<Bit<T>>* res = new BitSequence<T>(*sub);
        delete sub;
        return res;
    }
    
    Sequence<Bit<T>>* Append(Bit<T> item) override {
        data->InsertAt(item, data->GetSize());
        return this;
    }
    
    Sequence<Bit<T>>* Prepend(Bit<T> item) override {
        data->InsertAt(item, 0);
        return this;
    }
    
    Sequence<Bit<T>>* InsertAt(Bit<T> item, size_t index) override {
        data->InsertAt(item, index);
        return this;
    }

    Sequence<Bit<T>>* DeleteAt(size_t index) override {
        if (index >= data->GetSize()) {
            throw IndexOutOfRangeException(index, data->GetSize());
        }
        data->DeleteAt(index);
        return this;
    }

    Sequence<Bit<T>>* Set(size_t index, Bit<T> value) override {
        if (index >= data->GetSize()) {
            throw IndexOutOfRangeException(index, data->GetSize());
        }
        data->Set(index, value);
        return this;
    }
    
    Sequence<Bit<T>>* Concat(Sequence<Bit<T>>* list) override {
        auto other = static_cast<BitSequence<T>*>(list);
        DynamicArray<Bit<T>>* bitArray = data->Concat(other->data);
        Sequence<Bit<T>>* res = new BitSequence<T>(*bitArray);
        delete bitArray;
        return res;
    }
    
    auto begin() const { return data->begin(); }
    auto end()   const { return data->end(); }
    
    template <typename T2>
    Sequence<T2>* Map(T2 (*func)(Bit<T>)) {
        ArraySequence<T2>* res = new ArraySequence<T2>();
        for (size_t i = 0; i < data->GetSize(); i++) {
            res->Append(func(data->Get(i)));
        }
        return res;
    }

    Sequence<Bit<T>>* Where(bool (*predicate)(Bit<T>)) {
        ArraySequence<Bit<T>>* res = new ArraySequence<Bit<T>>();
        for (size_t i = 0; i < data->GetSize(); i++) {
            Bit<T> val = data->Get(i);
            if (predicate(val)) {
                res->Append(val);
            }
        }
        return res;
    }
    
    template<typename T2>
    T2 Reduce(T2 (*func)(T2, Bit<T>), T2 init) {
        T2 acc = init;
        for (size_t i = 0; i < data->GetSize(); i++) {
            acc = func(acc, data->Get(i));
        }
        return acc;
    }
    
    BitSequence<T> operator&(const BitSequence<T>& other) const {
        if (data->GetSize() != other.data->GetSize()) {
            throw IndexOutOfRangeException(data->GetSize(), other.data->GetSize());
        }
        BitSequence<T> res;
        res.data->Resize(data->GetSize());
        for (size_t i = 0; i < data->GetSize(); i++) {
            res.data->Set(i, data->Get(i) & other.data->Get(i));
        }
        return res;
    }
    
    BitSequence<T> operator|(const BitSequence<T>& other) const {
        if (data->GetSize() != other.data->GetSize()) {
            throw IndexOutOfRangeException(data->GetSize(), other.data->GetSize());
        }
        BitSequence<T> res;
        res.data->Resize(data->GetSize());
        for (size_t i = 0; i < data->GetSize(); i++) {
            res.data->Set(i, data->Get(i) | other.data->Get(i));
        }
        return res;
    }
    
    BitSequence<T> operator^(const BitSequence<T>& other) const {
        if (data->GetSize() != other.data->GetSize()) {
            throw IndexOutOfRangeException(data->GetSize(), other.data->GetSize());
        }
        BitSequence<T> res;
        res.data->Resize(data->GetSize());
        for (size_t i = 0; i < data->GetSize(); i++) {
            res.data->Set(i, data->Get(i) ^ other.data->Get(i));
        }
        return res;
    }
    
    BitSequence<T> operator~() const {
        BitSequence<T> res;
        res.data->Resize(data->GetSize());
        for (size_t i = 0; i < data->GetSize(); i++) {
            res.data->Set(i, ~data->Get(i));
        }
        return res;
        
    }

    std::string ToString() override {
        std::string res = "[";
        size_t totalBits = GetLength() * Bit<T>::BitCount();
        for (size_t i = 0; i < totalBits; i++) {
            if (i > 0) {
                res += ", ";
            }
            res += ((*this)[i] ? '1' : '0');
        }
        res += "]";
        return res;
    }

    std::string ToString(Bit<T> bit) override {
        return bit ? "1" : "0";
    }

    Bit<T> FromString(const std::string& s) override {
        if (s.empty()) {
            throw InvalidInputException("Строка пустая");
        }

        T value = 0;
        for (char c : s) {
            if (c == '0') {
                value <<= 1;
            } else if (c == '1') {
                value = (value << 1) | static_cast<T>(1);
            } else {
                std::string err = "Неверный символ '";
                err += c;
                err += "'. Допустимы только '0' и '1'";
                throw InvalidInputException(err);
            }
        }
        return Bit<T>(value);
    }
};

}