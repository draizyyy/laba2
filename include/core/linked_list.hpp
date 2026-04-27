#pragma once
#include "exceptions.hpp"

template<typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(T value, Node* prev, Node* next) : value(value), prev(prev), next(next) {}
        Node() : data(nullptr), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;
public:
    LinkedList(T* items, int count) : head(nullptr), tail(nullptr), size(0) {
        if (!items || count <= 0) {
            throw IndexOutOfRangeException();
        }
        for (int i = 0; i < count; i++) {
            this->Append(items[i]);
        }
    }
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    LinkedList(const LinkedList<T>& other) : size(other.size) {
        Node* curr = new Node();
    }
    ~LinkedList() {
        Node* curr = head;
        for (int i = 0; i < size; i++) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    
    void Append(T item) {
        Node* node = new Node(item, nullptr, nullptr);

        if (size <= 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }

    T Get(int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException();
        }
        Node* curr;
        if (index < (size / 2)) {
            curr = head;
            for (int i = 0; i < index; i++) {
                curr = curr->next;
            }
        } else {
            curr = tail;
            for (int i = size - 1; i > index; i--) {
                curr = curr->prev;
            }
        }
        return curr->value;
    }
};