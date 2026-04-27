#include <iostream>
#include "core/linked_list.hpp"

int main() {
    int arr[] = {1, 2, 3, 4, 5};    
    LinkedList<int> list(arr, 5);

    list.Append(777);

    for (int i = 0; i < 6; i++) {
        std::cout << list.Get(i) << " ";
    }
}
