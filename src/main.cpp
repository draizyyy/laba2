#include <iostream>
#include "core/linked_list.hpp"
using namespace myLib;

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};    
    LinkedList<int> list(arr, 9);
    LinkedList<int>* subList = list.GetSubList(1, 7);
    subList->InsertAt(777, 6);

    for (int i = 0; i < 10; i++) {
        std::cout << subList->Get(i) << " ";
    }
}
