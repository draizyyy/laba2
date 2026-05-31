#pragma once
#include "core/linked_list.hpp"
#include "sequence.hpp"
#include "option.hpp"
#include <string>
#include <sstream>
#include <format>
#include "exceptions.hpp"

namespace myLib {
template<typename T>
class ListSequence : public Sequence<T> {
private:
    std::string elementToString(T el) {
        return std::format("{}", el);
    }

protected:
    LinkedList<T>* data;
    size_t size{};

public:
    ListSequence() : data(new LinkedList<T>()) {}
    ListSequence(T* items, size_t count) : data(new LinkedList<T>(items, count)), size(count) {}
        ListSequence(LinkedList<T>& list) : data(new LinkedList<T>(list)) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            size++;
        }
    }
    ~ListSequence() override {
        delete data;
    }

    T GetFirst() override { 
        return data->GetFirst(); 
    }
    T GetLast() override { 
        return data->GetLast(); 
    }
    T Get(size_t index) override { 
        return data->Get(index); 
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override {
        return new ListSequence<T>(*data->GetSubList(startIndex, endIndex));
    }

    size_t GetLength() override {
        return size;
    }

    Sequence<T>* Append(T item) override {
        data->Append(item);
        size++;
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        data->Prepend(item);
        size++;
        return this;
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        data->InsertAt(item, index);
        size++;
        return this;
    }

    Sequence<T>* DeleteAt(size_t index) override {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        data->DeleteAt(index);
        size--;
        return this;
    }

    Sequence<T>* Set(size_t index, T value) override {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        data->DeleteAt(index);
        data->InsertAt(value, index);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        if (list->GetLength() == 0) {
            return new ListSequence<T>(*data);
        }
        
        LinkedList<T>* curr = new LinkedList<T>(*data);
        auto other = static_cast<ListSequence<T>*>(list);
        for (const auto& el : *other) {
            curr->Append(el);
        }
        
        Sequence<T>* res = new ListSequence<T>(*curr);
        delete curr;
        return res;
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
        size_t i = 0;
        for (const auto& val : *this) {
            t2[i++] = func(val);
        }
        Sequence<T2>* seq = new ListSequence<T2>(t2, len);
        delete[] t2;
        return seq;
    }

    Sequence<T>* Where(bool (*predicate)(T)) {
        T* t = new T[GetLength()];
        size_t count = 0;
        for (const auto& val : *this) {
            if (predicate(val)) {
                t[count++] = val;
            }
        }
        Sequence<T>* seq = new ListSequence<T>(t, count);
        delete[] t;
        return seq;
    }

    template<typename T2>
    Sequence<T2>* Reduce(T2 (*func)(T2, T), T2 t2) {
        T2 t = t2;
        for (const auto& val : *this) {
            t = func(t, val);
        }
        T2 arr[1] = { t };
        return new ListSequence<T2>(arr, 1);
    }

    Option<T> GetFirst(bool (*predicate)(T)) {
        for (const auto& val : *this) {
            if (predicate == nullptr || predicate(val)) {
                return Option<T>(val);
            }
        }
        return Option<T>();
    }

    Option<T> GetLast(bool (*predicate)(T)) {
        Option<T> result;
        for (const auto& val : *this) {
            if (predicate == nullptr || predicate(val)) {
                result = Option<T>(val);
            }
        }
        return result;
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
            oss << elementToString(el);
            first = false;
        }
        oss << "]";
        return oss.str();
    }

    std::string ToString(T elem) override {
        return elementToString(elem);
    }

    T FromString(const std::string& s) {
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
}