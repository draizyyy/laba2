#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include "option.hpp"
#include <string>
#include <sstream>
#include <format>
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class ArraySequence : public Sequence<T> {
private:
    std::string elementToString(T el) {
        return std::format("{}", el);
    }
protected:
    DynamicArray<T>* data;
    virtual ArraySequence<T>* Clone() {
        return new ArraySequence<T>(*data);
    }
    virtual ArraySequence<T>* Instance() {
        return this;
    }

    Sequence<T>* AppendDefault(T item, ArraySequence<T>* seq) {
        seq->data->InsertAt(item, seq->GetLength());
        return seq;
    }

    Sequence<T>* PrependDefault(T item, ArraySequence<T>* seq) {
        seq->data->InsertAt(item, 0);
        return seq;
    }

    Sequence<T>* InsertAtDefault(T item, size_t index, ArraySequence<T>* seq) {
        if (index > seq->GetLength()) {
            throw IndexOutOfRangeException(index, seq->GetLength());
        }
        seq->data->InsertAt(item, index);
        return seq;
    }

    Sequence<T>* ConcatDefault(Sequence<T>* list, ArraySequence<T>* seq) {
        auto arr = static_cast<ArraySequence<T>*>(list); 
        DynamicArray<T>* res = seq->data->Concat(arr->data);
        Sequence<T>* result = new ArraySequence<T>(*res);
        delete res;
        return result;
    }
public:
    ArraySequence() : data(new DynamicArray<T>()) {};
    ArraySequence(T* items, size_t count) : data(new DynamicArray<T>(items, count)) {};
    ArraySequence(const DynamicArray<T>& list) : data(new DynamicArray<T>(list)) {}
    ~ArraySequence() override {
        delete data;
    }

    T GetFirst() override {
        return data->Get(0);
    }

    T GetLast() override {
        return data->Get(data->GetSize()-1);
    }

    T Get(size_t index) override {
        return data->Get(index);
    }
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override {
        DynamicArray<T>* sub = data->SubArray(startIndex, endIndex);
        Sequence<T>* seq = new ArraySequence<T>(*sub);
        delete sub;
        return seq;
    }  
    size_t GetLength() override {
        return data->GetSize();
    }

    Sequence<T>* Append(T item) override {
        return AppendDefault(item, Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return PrependDefault(item, Instance());
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        return InsertAtDefault(item, index, Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return ConcatDefault(list, Clone());
    }

    auto begin() const { 
        return data->begin();
    }
    auto end() const { 
        return data->end(); 
    }

    template<typename T2>
    Sequence<T2>* Map(T2 (*func)(T)) {
        size_t len = GetLength();
        T2* t2 = new T2[len];
        for (size_t i = 0; i < len; i++) {
            t2[i] = func(Get(i));
        }
        Sequence<T2>* seq = new ArraySequence<T2>(t2, len);
        delete[] t2;
        return seq;
    }

    Sequence<T>* Where(bool (*predicate)(T)) {
        size_t len = GetLength();
        T* t = new T[len];
        size_t count = 0;
        for (size_t i = 0; i < len; i++) {
            T t2 = Get(i);
            if (predicate(t2)) {
                t[count++] = t2;
            }
        }
        Sequence<T>* seq = new ArraySequence<T>(t, count);
        delete[] t;
        return seq;
    }

    template<typename T2>
    Sequence<T2>* Reduce(T2 (*func)(T2, T), T2 t2) {
        T2 t = t2;
        size_t len = GetLength();
        for (size_t i = 0; i < len; i++) {
            t = func(t, Get(i));
        }
        T2 arr[1] = {t};
        return new ArraySequence<T2>(arr, 1);
    }

    Option<T> GetFirst(bool (*predicate)(T))  {
        size_t len = GetLength();
        for (size_t i = 0; i < len; i++) {
            T t = Get(i);
            if (predicate == nullptr || predicate(t)) {
                return Option<T>(t);
            }
        }
        return Option<T>();
    }

    Option<T> GetLast(bool (*predicate)(T)) {
        size_t len = GetLength();
        for (size_t i = len - 1; i >= 0; i--) {
            T t = Get(i);
            if (predicate == nullptr || predicate(t)) {
                return Option<T>(t);
            }
        }
        return Option<T>();
    }

    Sequence<double>* GetMinMaxAvg() {
        if (GetLength() == 0) {
            throw EmptyCollectionException("ArraySequence");
        }
        
        T minVal = Get(0);
        T maxVal = Get(0);
        double sum = 0;
        
        for (const auto& el : *this) {
            if (el < minVal) minVal = el;
            if (el > maxVal) maxVal = el;
            sum += static_cast<double>(el);
        }
        
        double avg = sum / GetLength();
        
        Sequence<double>* res = new ArraySequence<double>();
        res->Append(minVal);
        res->Append(maxVal);
        res->Append(avg);
        return res;
    }

    Sequence<Sequence<T>*>* GetPrefixes() {
        auto result = new ArraySequence<Sequence<T>*>();
        size_t i = 0;
        for (const auto& el : *this) {
            result->Append(GetSubsequence(0, i++));
        }
        return result;
    }

    Sequence<T>* GetReflectionSum() {
        auto result = new ArraySequence<T>();
        size_t len = GetLength();
        size_t i = 0;
        
        for (const auto& el : *this) {
            result->Append(el + Get(len - 1 - i));
            i++;
        }
        return result;
    }

    std::string ToString() override {
        std::ostringstream oss;
        oss << "[";
        bool first = true;
        for (const auto& el : *this) {
            if (!first) oss << ", ";
            oss << el; 
            first = false;
        }
        oss << "]";
        return oss.str();
    }

    std::string ToString(T elem) override {
        return elementToString(elem);
    }

    T FromString(const std::string& s) override {
        if (s.empty()) {
            throw InvalidInputException("Строка пустая");
        }
        
        std::istringstream iss(s);
        T value;
        iss >> value;

        if (iss.fail()) {
            throw InvalidInputException("Неверный формат данных");
        }
        
        return value;
    }
};

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
    protected:
    ArraySequence<T>* Instance() override {
        return this;
    }
    public:
    using ArraySequence<T>::ArraySequence;
};

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
    protected:
    virtual ArraySequence<T>* Clone() override {
        return new ImmutableArraySequence<T>(*this->data);
    }

    ArraySequence<T>* Instance() override {
        return Clone();
    }

    public:
    using ArraySequence<T>::ArraySequence;

    Sequence<T>* Append(T item) override {
        return this->AppendDefault(item, this->Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return this->PrependDefault(item, this->Instance());
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        return this->InsertAtDefault(item, index, this->Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return this->ConcatDefault(list, this->Instance());
    }
};

}