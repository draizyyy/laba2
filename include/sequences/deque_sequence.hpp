#pragma once
#include "sequences/sequence.hpp"
#include "core/linked_list.hpp"
#include "linked_list.hpp"


namespace myLib {

template<typename T>
class DequeSequence : public Sequence<T> {
private:
    LinkedList<T>* data;

public:
    DequeSequence() : data(new LinkedList<T>()) {}
    // доделать копирующий конструктор
    DequeSequence(T* items, int count) : data(new LinkedList<T>(items, count)) {}
    
    ~DequeSequence() override {
        delete data;
    }

    void PushFront(T item) {
        data->Prepend(item);
    }

    void PushBack(T item) {
        data->Append(item);
    }
// // +delete в linked list
//     T PopFront() {
//         T val = data->GetFirst();

//         return val;
//     }

//     T PopBack() {
//         T val = data->GetLast();
        
//         return val;
//     }

    T GetFront() {
        return data->GetFirst();
    }

    T GetBack() {
        return data->GetLast();
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
    int GetLength() override { 
        return data->GetLength(); 
    }

    Sequence<T>* GetSubsequence(int start, int end) override {
        return new DequeSequence<T>(*data->GetSubList(start, end));
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

    Sequence<T>* Concat(Sequence<T>* other) override {
        DequeSequence<T>* deque = new DequeSequence<T>();
        for (int i = 0; i < GetLength(); i++)
            deque->Append(Get(i));
        for (int i = 0; i < other->GetLength(); i++)
            deque->Append(other->Get(i));
        return deque;
    }

    // // сначала надо сделать swap в linked list
    // void Sort() {
        
    // }

    bool ContainsSubsequence(Sequence<T>* subSeq) {
        int len = GetLength();
        int subLen = subSeq->GetLength();
        
        for (int i = 0; i <= (len - subLen); i++) {
            bool fl = true;
            for (int j = 0; j < subLen; j++) {
                if (Get(i + j) != subSeq->Get(j)) {
                    fl = false;
                    break;
                }
            }
            if (fl) {
                return true;
            }
        }
        return false;
    }

    IEnumerator<T>* GetEnumerator() override {
        return new ListEnumerator<T>(data);
    }

    // template<typename T2>
    // Sequence<T2>* Map(T2 (*func)(T)) {

    // }

    // Sequence<T>* Where(bool (*predicate)(T)) {

    // }

    // template<typename T2>
    // T2 Reduce(T2 (*func)(T2, T), T2 t) {

    // }

};

}