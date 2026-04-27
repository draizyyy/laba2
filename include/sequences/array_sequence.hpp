// template <class T>
// class ArraySequence : public Sequence<T> {
// private:
//     DynamicArray<T>* items;

// protected:
//     virtual Sequence<T>* Instance() = 0;
    
//     Sequence<T>* AppendInternal(T item) {
//         this->items->Resize(this->items->GetSize() + 1);
//         this->items->Set(this->items->GetSize() - 1, item);
//         return this;
//     }

// public:
//     ArraySequence() : items(new DynamicArray<T>()) {}
//     ArraySequence(T* arr, int count) : items(new DynamicArray<T>(arr, count)) {}
    

//     Sequence<T>* Append(T item) override {
//         return Instance()->AppendInternal(item);
//     }
    
// };