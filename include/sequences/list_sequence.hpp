#pragma once
#include "core/linked_list.hpp"

namespace myLib {
template<typename T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T>* data;
public:
    ListSequence () : data(new LinkedList<T>(0)) {}
    ListSequence (T* items, int count) : data(new LinkedList<T>(items, count)) {}
    ListSequence(LinkedList<T>& list) : data(LinkedList(list)) {}

    ~ListSequence() override {
        delete data;
    }

    T GetFirst() override {
        return data->GetFirst();
    }
    T GetLast() override {
        return data->GetLast();
    }
    T Get(int index) override {
        return data->Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) override {
        Sequence<T>* seq = new ListSequence<T>(data->GetSubList(startIndex, endIndex));
        return seq;
    }

    int GetLength() override {
        return data->GetLength();
    }

    Sequence<T>* Append(T item) override {
        data->Append(item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        data->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        data->InsertAt(item, index);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        LinkedList<T>* newList = new LinkedList<T>(*data);
        for (int i = 0; i < list->GetLength(); i++) {
            newList->Append(list->Get(i));
        }
        Sequence<T>* res = new ListSequence<T>(*newList);
        delete newList;
        return res;
    }
};
}