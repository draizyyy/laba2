// #include <gtest/gtest.h>
// #include <string>
// #include <format>
// #include "core/linked_list.hpp"
// #include "core/dynamic_array.hpp"
// #include "exceptions.hpp"
// #include "sequences/array_sequence.hpp"
// #include "sequences/list_sequence.hpp"
// #include "sequences/sequence.hpp"
// #include "option.hpp"
// #include "sequences/bit_sequence.hpp"


// using namespace myLib;

// static int TEST_ARR[] = {10, 20, 30, 40, 50};
// static int TEST_ARR_LEN = 5;

// // LinkedList

// TEST(LinkedListTest, DefaultConstructor) {
//     LinkedList<int> list;
//     EXPECT_THROW(list.GetFirst(), EmptyCollectionException);
//     EXPECT_THROW(list.GetLast(), EmptyCollectionException);
// }

// TEST(LinkedListTest, ArrayConstructor) {
//     LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
//     EXPECT_EQ(list.GetFirst(), 10);
//     EXPECT_EQ(list.GetLast(), 50);
//     EXPECT_EQ(list.Get(2), 30);
// }

// TEST(LinkedListTest, CopyConstructor) {
//     LinkedList<int> original(TEST_ARR, 3);
//     LinkedList<int> copy(original);
    
//     EXPECT_EQ(copy.Get(0), 10);
//     EXPECT_EQ(copy.Get(2), 30);
    
//     original.Append(999);
//     EXPECT_EQ(original.Get(3), 999);
//     EXPECT_THROW(copy.Get(3), IndexOutOfRangeException);
// }

// TEST(LinkedListTest, GetFirstAndGetLast) {
//     LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
//     EXPECT_EQ(list.GetFirst(), 10);
//     EXPECT_EQ(list.GetLast(), 50);
// }

// TEST(LinkedListTest, GetByIndex_Valid) {
//     LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
//     EXPECT_EQ(list.Get(0), 10);
//     EXPECT_EQ(list.Get(4), 50);
//     EXPECT_EQ(list.Get(2), 30);
// }

// TEST(LinkedListTest, GetByIndex_Invalid) {
//     LinkedList<int> list(TEST_ARR, 3);
//     EXPECT_THROW(list.Get(3), IndexOutOfRangeException);
//     EXPECT_THROW(list.Get(100), IndexOutOfRangeException);
//     EXPECT_THROW(list.Get(static_cast<size_t>(-1)), IndexOutOfRangeException);
// }

// TEST(LinkedListTest, GetSubList_Valid) {
//     LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    
//     LinkedList<int>* sub = list.GetSubList(1, 3);
//     ASSERT_NE(sub, nullptr);
//     EXPECT_EQ(sub->Get(0), 20);
//     EXPECT_EQ(sub->Get(1), 30);
//     delete sub;
// }

// TEST(LinkedListTest, GetSubList_InvalidIndices) {
//     LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
//     EXPECT_THROW(list.GetSubList(0, 6), IndexOutOfRangeException);
//     EXPECT_THROW(list.GetSubList(3, 1), IndexOutOfRangeException);
// }

// TEST(LinkedListTest, Append) {
//     LinkedList<int> list;
//     list.Append(10);
//     list.Append(20);
//     list.Append(30);
    
//     EXPECT_EQ(list.GetFirst(), 10);
//     EXPECT_EQ(list.GetLast(), 30);
//     EXPECT_EQ(list.Get(1), 20);
// }

// TEST(LinkedListTest, Prepend) {
//     LinkedList<int> list;
//     list.Prepend(30);
//     list.Prepend(20);
//     list.Prepend(10);
    
//     EXPECT_EQ(list.GetFirst(), 10);
//     EXPECT_EQ(list.GetLast(), 30);
// }

// TEST(LinkedListTest, InsertAt_Valid) {
//     LinkedList<int> list;
//     list.Append(10);
//     list.Append(30);
    
//     list.InsertAt(20, 1);
//     EXPECT_EQ(list.Get(0), 10);
//     EXPECT_EQ(list.Get(1), 20);
//     EXPECT_EQ(list.Get(2), 30);
    
//     list.InsertAt(5, 0);
//     EXPECT_EQ(list.Get(0), 5);
    
//     list.InsertAt(50, 4);
//     EXPECT_EQ(list.GetLast(), 50);
// }

// TEST(LinkedListTest, InsertAt_Invalid) {
//     LinkedList<int> list;
//     list.Append(10);
    
//     EXPECT_THROW(list.InsertAt(99, 2), IndexOutOfRangeException);
//     EXPECT_THROW(list.InsertAt(99, static_cast<size_t>(-1)), IndexOutOfRangeException);
// }

// TEST(LinkedListTest, Concat) {
//     int arr1[] = {1, 2};
//     int arr2[] = {3, 4, 5};
//     LinkedList<int> list1(arr1, 2);
//     LinkedList<int> list2(arr2, 3);
    
//     LinkedList<int>* concat = list1.Concat(&list2);
//     ASSERT_NE(concat, nullptr);
//     EXPECT_EQ(concat->Get(0), 1);
//     EXPECT_EQ(concat->Get(2), 3);
//     EXPECT_EQ(concat->Get(4), 5);
    
//     EXPECT_EQ(list1.Get(1), 2);
//     EXPECT_EQ(list2.Get(2), 5);
    
//     delete concat;
// }

// TEST(LinkedListTest, Concat_EmptyList) {
//     LinkedList<int> list;
//     int arr[] = {1, 2};
//     LinkedList<int> other(arr, 2);
    
//     LinkedList<int>* res1 = list.Concat(&other);
//     EXPECT_EQ(res1->Get(0), 1);
//     EXPECT_EQ(res1->Get(1), 2);
//     delete res1;
    
//     LinkedList<int>* res2 = other.Concat(&list);
//     EXPECT_EQ(res2->GetLast(), 2);
//     delete res2;
// }

// // DynamicArray

// static int DA_TEST_ARR[] = {100, 200, 300, 400, 500};
// static int DA_TEST_LEN = 5;

// TEST(DynamicArrayTest, ArrayConstructor) {
//     DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
//     EXPECT_EQ(arr.GetSize(), DA_TEST_LEN);
//     EXPECT_EQ(arr.Get(0), 100);
//     EXPECT_EQ(arr.Get(2), 300);
//     EXPECT_EQ(arr.Get(4), 500);
// }

// TEST(DynamicArrayTest, SizeConstructor) {
//     DynamicArray<int> arr(10);
//     EXPECT_EQ(arr.GetSize(), 10);
// }

// TEST(DynamicArrayTest, CopyConstructor) {
//     DynamicArray<int> original(DA_TEST_ARR, 3);
//     DynamicArray<int> copy(original);
    
//     EXPECT_EQ(copy.GetSize(), 3);
//     EXPECT_EQ(copy.Get(0), 100);
//     EXPECT_EQ(copy.Get(2), 300);
    
//     original.Set(0, 999);
//     EXPECT_EQ(original.Get(0), 999);
//     EXPECT_EQ(copy.Get(0), 100);
// }

// TEST(DynamicArrayTest, Get_Valid) {
//     DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
//     EXPECT_EQ(arr.Get(0), 100);
//     EXPECT_EQ(arr.Get(4), 500);
//     EXPECT_EQ(arr.Get(2), 300);
// }

// TEST(DynamicArrayTest, Get_Invalid) {
//     DynamicArray<int> arr(DA_TEST_ARR, 3);
//     EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
//     EXPECT_THROW(arr.Get(100), IndexOutOfRangeException);
//     EXPECT_THROW(arr.Get(static_cast<size_t>(-1)), IndexOutOfRangeException);
// }

// TEST(DynamicArrayTest, GetSize) {
//     DynamicArray<int> arr1(0);
//     EXPECT_EQ(arr1.GetSize(), 0);
    
//     DynamicArray<int> arr2(DA_TEST_ARR, DA_TEST_LEN);
//     EXPECT_EQ(arr2.GetSize(), DA_TEST_LEN);
// }

// TEST(DynamicArrayTest, Set_Valid) {
//     DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
//     arr.Set(0, 1000);
//     arr.Set(2, 3000);
//     arr.Set(4, 5000);
    
//     EXPECT_EQ(arr.Get(0), 1000);
//     EXPECT_EQ(arr.Get(2), 3000);
//     EXPECT_EQ(arr.Get(4), 5000);
// }

// TEST(DynamicArrayTest, Set_Invalid) {
//     DynamicArray<int> arr(DA_TEST_ARR, 3);
//     EXPECT_THROW(arr.Set(3, 100), IndexOutOfRangeException);
//     EXPECT_THROW(arr.Set(100, 100), IndexOutOfRangeException);
// }

// TEST(DynamicArrayTest, Resize_Larger) {
//     DynamicArray<int> arr(DA_TEST_ARR, 3);
//     arr.Resize(5);
    
//     EXPECT_EQ(arr.GetSize(), 5);
//     EXPECT_EQ(arr.Get(0), 100);
//     EXPECT_EQ(arr.Get(1), 200);
//     EXPECT_EQ(arr.Get(2), 300);
// }

// TEST(DynamicArrayTest, Resize_Smaller) {
//     DynamicArray<int> arr(DA_TEST_ARR, 5);
//     arr.Resize(3);
    
//     EXPECT_EQ(arr.GetSize(), 3);
//     EXPECT_EQ(arr.Get(0), 100);
//     EXPECT_EQ(arr.Get(1), 200);
//     EXPECT_EQ(arr.Get(2), 300);
//     EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
// }

// TEST(DynamicArrayTest, Resize_Same) {
//     DynamicArray<int> arr(DA_TEST_ARR, 3);
//     arr.Resize(3);
    
//     EXPECT_EQ(arr.GetSize(), 3);
//     EXPECT_EQ(arr.Get(0), 100);
//     EXPECT_EQ(arr.Get(2), 300);
// }

// TEST(DynamicArrayTest, Resize_Zero) {
//     DynamicArray<int> arr(DA_TEST_ARR, 3);
//     arr.Resize(0);
    
//     EXPECT_EQ(arr.GetSize(), 0);
//     EXPECT_THROW(arr.Get(0), IndexOutOfRangeException);
// }

// TEST(DynamicArrayTest, Resize_PreservesData) {
//     int arr[] = {1, 2, 3, 4, 5};
//     DynamicArray<int> da(arr, 5);
    
//     da.Resize(10);
//     da.Set(5, 60);
//     da.Set(9, 100);
    
//     EXPECT_EQ(da.Get(0), 1);
//     EXPECT_EQ(da.Get(4), 5);
//     EXPECT_EQ(da.Get(5), 60);
//     EXPECT_EQ(da.Get(9), 100);
//     EXPECT_EQ(da.GetSize(), 10);
// }

// TEST(DynamicArrayTest, EmptyArray) {
//     DynamicArray<int> arr(0);
//     EXPECT_EQ(arr.GetSize(), 0);
//     EXPECT_THROW(arr.Get(0), IndexOutOfRangeException);
//     EXPECT_THROW(arr.Set(0, 10), IndexOutOfRangeException);
// }

// // SEQUENCE

// static int SEQ_TEST_ARR[] = {1, 2, 3, 4, 5};
// static int SEQ_TEST_LEN = 5;

// // ArraySequence

// TEST(ArraySequenceTest, ArrayConstructor) {
//     ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
//     EXPECT_EQ(seq.GetLength(), SEQ_TEST_LEN);
//     EXPECT_EQ(seq.Get(0), 1);
//     EXPECT_EQ(seq.Get(4), 5);
// }

// TEST(ArraySequenceTest, GetFirstLast) {
//     ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
//     EXPECT_EQ(seq.GetFirst(), 1);
//     EXPECT_EQ(seq.GetLast(), 5);
// }

// TEST(ArraySequenceTest, GetSubsequence) {
//     ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
//     Sequence<int>* sub = seq.GetSubsequence(1, 4);
//     ASSERT_NE(sub, nullptr);
//     EXPECT_EQ(sub->GetLength(), 3);
//     EXPECT_EQ(sub->Get(0), 2);
//     EXPECT_EQ(sub->Get(2), 4);
//     delete sub;
// }

// TEST(ArraySequenceTest, AppendPrependInsertAt_MutableBehavior) {
//     ArraySequence<int> seq;
//     seq.Append(2);
//     seq.Append(3);
//     seq.Prepend(1);
//     seq.InsertAt(4, 3);
    
//     EXPECT_EQ(seq.GetLength(), 4);
//     EXPECT_EQ(seq.Get(0), 1);
//     EXPECT_EQ(seq.Get(1), 2);
//     EXPECT_EQ(seq.Get(2), 3);
//     EXPECT_EQ(seq.Get(3), 4);
// }

// TEST(ArraySequenceTest, Concat_ReturnsNewSequence) {
//     int arr1[] = {1, 2};
//     int arr2[] = {3, 4, 5};
//     ArraySequence<int> seq1(arr1, 2);
//     ArraySequence<int> seq2(arr2, 3);
    
//     Sequence<int>* res = seq1.Concat(&seq2);

//     EXPECT_NE(static_cast<void*>(res), static_cast<void*>(&seq1));
//     EXPECT_EQ(seq1.GetLength(), 2);
//     EXPECT_EQ(res->GetLength(), 5);
//     EXPECT_EQ(res->Get(4), 5);
    
//     delete res;
// }

// // Mutable

// TEST(MutableArraySequenceTest, Append_ModifiesOriginal) {
//     MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq.Append(999);
    
//     EXPECT_EQ(seq.GetLength(), 4);
//     EXPECT_EQ(seq.Get(3), 999);
//     EXPECT_EQ(static_cast<void*>(res), static_cast<void*>(&seq));
// }

// TEST(MutableArraySequenceTest, Prepend_ModifiesOriginal) {
//     MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq.Prepend(0);
    
//     EXPECT_EQ(seq.GetLength(), 4);
//     EXPECT_EQ(seq.Get(0), 0);
//     EXPECT_EQ(static_cast<void*>(res), static_cast<void*>(&seq));
// }

// TEST(MutableArraySequenceTest, InsertAt_ModifiesOriginal) {
//     MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq.InsertAt(99, 1);
    
//     EXPECT_EQ(seq.GetLength(), 4);
//     EXPECT_EQ(seq.Get(1), 99);
//     EXPECT_EQ(static_cast<void*>(res), static_cast<void*>(&seq));
// }

// TEST(MutableArraySequenceTest, ChainOperations) {
//     MutableArraySequence<int> seq;
//     seq.Append(1)->Append(2)->Append(3);
    
//     EXPECT_EQ(seq.GetLength(), 3);
//     EXPECT_EQ(seq.Get(0), 1);
//     EXPECT_EQ(seq.Get(2), 3);
// }

// TEST(MutableArraySequenceTest, InsertAt_InvalidIndex) {
//     MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     EXPECT_THROW(seq.InsertAt(99, 4), IndexOutOfRangeException);
//     EXPECT_THROW(seq.InsertAt(99, static_cast<size_t>(-1)), IndexOutOfRangeException);
// }

// // Immutable

// TEST(ImmutableArraySequenceTest, Append_CreatesNewSequence) {
//     ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq.Append(999);
    
//     EXPECT_EQ(seq.GetLength(), 3);
//     EXPECT_EQ(seq.Get(0), 1);
//     EXPECT_EQ(seq.Get(2), 3);
    
//     EXPECT_EQ(res->GetLength(), 4);
//     EXPECT_EQ(res->Get(3), 999);
//     EXPECT_NE(static_cast<void*>(res), static_cast<void*>(&seq));
    
//     delete res;
// }

// TEST(ImmutableArraySequenceTest, Prepend_CreatesNewSequence) {
//     ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq.Prepend(0);
    
//     EXPECT_EQ(seq.GetLength(), 3);
//     EXPECT_EQ(seq.Get(0), 1);
    
//     EXPECT_EQ(res->GetLength(), 4);
//     EXPECT_EQ(res->Get(0), 0);
    
//     delete res;
// }

// TEST(ImmutableArraySequenceTest, InsertAt_CreatesNewSequence) {
//     ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq.InsertAt(99, 1);
    
//     EXPECT_EQ(seq.GetLength(), 3);
//     EXPECT_EQ(seq.Get(1), 2);
    
//     EXPECT_EQ(res->GetLength(), 4);
//     EXPECT_EQ(res->Get(1), 99);
    
//     delete res;
// }

// TEST(ImmutableArraySequenceTest, Concat_CreatesNewSequence) {
//     int arr1[] = {1, 2, 3};
//     int arr2[] = {4, 5};
//     ImmutableArraySequence<int> seq1(arr1, 3);
//     ImmutableArraySequence<int> seq2(arr2, 2);
    
//     Sequence<int>* res = seq1.Concat(&seq2);
    
//     EXPECT_EQ(seq1.GetLength(), 3);
//     EXPECT_EQ(res->GetLength(), 5);
//     EXPECT_EQ(res->Get(0), 1);
//     EXPECT_EQ(res->Get(4), 5);
    
//     delete res;
// }

// // Many Immutables
// TEST(ImmutableArraySequenceTest, ChainOperations_Safe) {
//     ImmutableArraySequence<int> seq;
//     Sequence<int>* s1 = seq.Append(1);
//     Sequence<int>* s2 = s1->Append(2);
//     Sequence<int>* s3 = s2->Append(3);
    
//     EXPECT_EQ(seq.GetLength(), 0);
//     EXPECT_EQ(s1->GetLength(), 1);
//     EXPECT_EQ(s2->GetLength(), 2);
//     EXPECT_EQ(s3->GetLength(), 3);
//     EXPECT_EQ(s3->Get(2), 3);
    
//     delete s1;
//     delete s2;
//     delete s3;
// }

// TEST(ImmutableArraySequenceTest, EmptySequence_Append) {
//     ImmutableArraySequence<int> seq;
//     Sequence<int>* res = seq.Append(1);
    
//     EXPECT_EQ(seq.GetLength(), 0);
//     EXPECT_EQ(res->GetLength(), 1);
//     EXPECT_EQ(res->Get(0), 1);
    
//     delete res;
// }

// TEST(ImmutableArraySequenceTest, InsertAt_InvalidIndex) {
//     ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     EXPECT_THROW(seq.InsertAt(99, 4), IndexOutOfRangeException);
// }

// // Mutable and Immutable

// TEST(MutableVsImmutableTest, Append_Behavior) {
//     MutableArraySequence<int> mutableSeq(SEQ_TEST_ARR, 3);
//     Sequence<int>* mutableResult = mutableSeq.Append(999);
    
//     ImmutableArraySequence<int> immutableSeq(SEQ_TEST_ARR, 3);
//     Sequence<int>* immutableResult = immutableSeq.Append(999);
    
//     EXPECT_EQ(mutableSeq.GetLength(), 4);
//     EXPECT_EQ(static_cast<void*>(mutableResult), static_cast<void*>(&mutableSeq));
    
//     EXPECT_EQ(immutableSeq.GetLength(), 3);
//     EXPECT_NE(static_cast<void*>(immutableResult), static_cast<void*>(&immutableSeq));
    
//     delete immutableResult;
// }

// TEST(MutableVsImmutableTest, Prepend_Behavior) {
//     MutableArraySequence<int> mutableSeq;
//     mutableSeq.Prepend(1);
//     mutableSeq.Prepend(2);
    
//     ImmutableArraySequence<int> immutableSeq;
//     Sequence<int>* s1 = immutableSeq.Prepend(1);
//     Sequence<int>* s2 = s1->Prepend(2);
    
//     EXPECT_EQ(mutableSeq.GetLength(), 2);
//     EXPECT_EQ(mutableSeq.Get(0), 2);
    
//     EXPECT_EQ(immutableSeq.GetLength(), 0);
//     EXPECT_EQ(s2->GetLength(), 2);
//     EXPECT_EQ(s2->Get(0), 2);
    
//     delete s1;
//     delete s2;
// }

// TEST(MutableVsImmutableTest, InsertAt_Behavior) {
//     MutableArraySequence<int> mutableSeq(SEQ_TEST_ARR, 3);
//     mutableSeq.InsertAt(99, 1);
    
//     ImmutableArraySequence<int> immutableSeq(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = immutableSeq.InsertAt(99, 1);
    
//     EXPECT_EQ(mutableSeq.Get(1), 99);
//     EXPECT_EQ(immutableSeq.Get(1), 2);
//     EXPECT_EQ(res->Get(1), 99);
    
//     delete res;
// }

// // Полиморфизм

// TEST(PolymorphismTest, MutableArraySequenceViaBasePointer) {
//     Sequence<int>* seq = new MutableArraySequence<int>(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq->Append(999);
    
//     EXPECT_EQ(res->GetLength(), 4);
//     EXPECT_EQ(res->Get(3), 999);
//     EXPECT_EQ(static_cast<void*>(res), static_cast<void*>(seq));
    
//     delete seq;
// }

// TEST(PolymorphismTest, ImmutableArraySequenceViaBasePointer) {
//     Sequence<int>* seq = new ImmutableArraySequence<int>(SEQ_TEST_ARR, 3);
//     Sequence<int>* res = seq->Append(999);
    
//     EXPECT_EQ(seq->GetLength(), 3);
//     EXPECT_EQ(res->GetLength(), 4);
//     EXPECT_NE(static_cast<void*>(res), static_cast<void*>(seq));
    
//     delete seq;
//     delete res;
// }

// // Exceptions
// TEST(SequenceExceptionsTest, Get_InvalidIndex) {
//     ArraySequence<int> seq(SEQ_TEST_ARR, 3);
//     EXPECT_THROW(seq.Get(3), IndexOutOfRangeException);
//     EXPECT_THROW(seq.Get(static_cast<size_t>(-1)), IndexOutOfRangeException);
// }

// TEST(SequenceExceptionsTest, GetSubsequence_Invalid) {
//     ListSequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
//     EXPECT_THROW(seq.GetSubsequence(0, 10), IndexOutOfRangeException);
// }

// TEST(SequenceExceptionsTest, EmptySequence) {
//     ArraySequence<int> seq;
//     EXPECT_THROW(seq.GetFirst(), IndexOutOfRangeException);
//     EXPECT_THROW(seq.GetLast(), IndexOutOfRangeException);
//     EXPECT_THROW(seq.Get(0), IndexOutOfRangeException);
// }

// // Iterator
// TEST(IteratorTest, ArraySequence_Traversal) {
//     ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    
//     int expected[] = {1, 2, 3, 4, 5};
//     size_t i = 0;
//     for (const auto& val : seq) {
//         EXPECT_EQ(val, expected[i++]);
//     }
//     EXPECT_EQ(i, SEQ_TEST_LEN);
// }

// TEST(IteratorTest, ListSequence_Traversal) {
//     ListSequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    
//     int sum = 0;
//     for (const auto& val : seq) {
//         sum += val;
//     }
//     EXPECT_EQ(sum, 15);
// }

// TEST(IteratorTest, EmptySequence) {
//     ArraySequence<int> seq;
    
//     size_t count = 0;
//     for (const auto& val : seq) {
//         (void)val;
//         count++;
//     }
//     EXPECT_EQ(count, 0);
// }

// // вспомогательные функции
// static int Square(int x) { return x * x; }
// static std::string ToString(int x) { return std::to_string(x); }
// static int Identity(int x) { return x; }
// static bool IsEven(int x) { return x % 2 == 0; }
// static bool IsPositive(int x) { return x > 0; }
// static int SumFunc(int acc, int x) { return acc + x; }
// static int ProdFunc(int acc, int x) { return acc * x; }
// static bool IsDivBy3(int x) { return x % 3 == 0; }

// static Bit<uint8_t> InvertLowNibble(Bit<uint8_t> b) { 
//     return Bit<uint8_t>(b.GetValue() ^ 0x0F); 
// }
// static bool HasBit0(Bit<uint8_t> b) { 
//     return b[0]; 
// }
// static int CountOnes(int acc, Bit<uint8_t> b) {
//     int count = 0;
//     uint8_t val = b.GetValue();
//     while (val) { 
//         count += val & 1; 
//         val >>= 1; 
//     }
//     return acc + count;
// }

// // BitSequence
// TEST(BitTest, ConstructorAndValue) {
//     Bit<uint8_t> b(0b10101010);
//     EXPECT_EQ(b.GetValue(), 0b10101010);
// }

// TEST(BitTest, BitCount) {
//     EXPECT_EQ(Bit<uint8_t>::BitCount(), 8);
//     EXPECT_EQ(Bit<uint16_t>::BitCount(), 16);
//     EXPECT_EQ(Bit<uint32_t>::BitCount(), 32);
// }

// TEST(BitTest, ReadBit_Const) {
//     Bit<uint8_t> b(0b10101010);
//     EXPECT_FALSE(static_cast<bool>(b[0])); 
//     EXPECT_TRUE(static_cast<bool>(b[1]));  
//     EXPECT_FALSE(static_cast<bool>(b[2]));  
//     EXPECT_TRUE(static_cast<bool>(b[7])); 
// }

// TEST(BitTest, WriteBit_ThroughProxy) {
//     Bit<uint8_t> b(0);
//     b[3] = true;
//     b[5] = true;
//     EXPECT_EQ(b.GetValue(), 0b00101000); 
//     EXPECT_TRUE(static_cast<bool>(b[3]));
//     EXPECT_TRUE(static_cast<bool>(b[5]));
//     EXPECT_FALSE(static_cast<bool>(b[0]));
// }

// TEST(BitTest, BitwiseAND) {
//     Bit<uint8_t> a(0b11001100);
//     Bit<uint8_t> b(0b10101010);
//     Bit<uint8_t> res = a & b;
//     EXPECT_EQ(res.GetValue(), 0b10001000);
// }

// TEST(BitTest, BitwiseOR) {
//     Bit<uint8_t> a(0b11000000);
//     Bit<uint8_t> b(0b00110000);
//     Bit<uint8_t> res = a | b;
//     EXPECT_EQ(res.GetValue(), 0b11110000);
// }

// TEST(BitTest, BitwiseXOR) {
//     Bit<uint8_t> a(0b11110000);
//     Bit<uint8_t> b(0b10101010);
//     Bit<uint8_t> res = a ^ b;
//     EXPECT_EQ(res.GetValue(), 0b01011010);
// }

// TEST(BitTest, BitwiseNOT) {
//     Bit<uint8_t> b(0b10101010);
//     Bit<uint8_t> res = ~b;
//     EXPECT_EQ(res.GetValue(), 0b01010101);
// }

// TEST(BitTest, ComparisonOperators) {
//     Bit<uint8_t> a(42), b(42), c(99);
//     EXPECT_TRUE(a == b);
//     EXPECT_FALSE(a == c);
//     EXPECT_TRUE(a != c);
//     EXPECT_FALSE(a != b);
// }

// TEST(BitTest, IndexOutOfRange) {
//     Bit<uint8_t> b(0);
//     EXPECT_THROW(b[8], IndexOutOfRangeException);
// }

// TEST(BitSequenceTest, ConstructorFromArray) {
//     Bit<uint8_t> bits[] = {
//         Bit<uint8_t>(0b10101010),
//         Bit<uint8_t>(0b01010101),
//         Bit<uint8_t>(0b11110000)
//     };
//     BitSequence<uint8_t> seq(bits, 3);
//     EXPECT_EQ(seq.GetLength(), 3);
//     EXPECT_EQ(seq.Get(0).GetValue(), 0b10101010);
//     EXPECT_EQ(seq.Get(1).GetValue(), 0b01010101);
//     EXPECT_EQ(seq.Get(2).GetValue(), 0b11110000);
// }

// TEST(BitSequenceTest, AppendPrependInsertAt) {
//     BitSequence<uint8_t> seq;
//     seq.Append(Bit<uint8_t>(0b1010));
//     seq.Prepend(Bit<uint8_t>(0b0101));
//     seq.InsertAt(Bit<uint8_t>(0b1111), 1);
//     EXPECT_EQ(seq.GetLength(), 3);
//     EXPECT_EQ(seq.Get(0).GetValue(), 0b0101);
//     EXPECT_EQ(seq.Get(1).GetValue(), 0b1111);
//     EXPECT_EQ(seq.Get(2).GetValue(), 0b1010);
// }

// TEST(BitSequenceTest, BitwiseOperators_SameLength) {
//     Bit<uint8_t> a1[] = { Bit<uint8_t>(0b1100), Bit<uint8_t>(0b1010) };
//     Bit<uint8_t> b1[] = { Bit<uint8_t>(0b1010), Bit<uint8_t>(0b0101) };
//     BitSequence<uint8_t> seqA(a1, 2);
//     BitSequence<uint8_t> seqB(b1, 2);
    
//     BitSequence<uint8_t> andRes = seqA & seqB;
//     EXPECT_EQ(andRes.Get(0).GetValue(), 0b1000);  
//     EXPECT_EQ(andRes.Get(1).GetValue(), 0b0000); 
    
//     BitSequence<uint8_t> orRes = seqA | seqB;
//     EXPECT_EQ(orRes.Get(0).GetValue(), 0b1110);   
//     EXPECT_EQ(orRes.Get(1).GetValue(), 0b1111);   
    
//     BitSequence<uint8_t> xorRes = seqA ^ seqB;
//     EXPECT_EQ(xorRes.Get(0).GetValue(), 0b0110);  
//     EXPECT_EQ(xorRes.Get(1).GetValue(), 0b1111);  
// }

// TEST(BitSequenceTest, BitwiseNOT) {
//     Bit<uint8_t> bits[] = { Bit<uint8_t>(0b10101010) };
//     BitSequence<uint8_t> seq(bits, 1);
//     BitSequence<uint8_t> notRes = ~seq;
//     EXPECT_EQ(notRes.Get(0).GetValue(), 0b01010101);
// }

// TEST(BitSequenceTest, BitMaskUsage) {
//     Bit<uint8_t> maskBits[] = { Bit<uint8_t>(0b11110000) };
//     BitSequence<uint8_t> mask(maskBits, 1);
//     Bit<uint8_t> dataBits[] = { Bit<uint8_t>(0b10101010) };
//     BitSequence<uint8_t> data(dataBits, 1);
//     BitSequence<uint8_t> masked = data & mask;
//     EXPECT_EQ(masked.Get(0).GetValue(), 0b10100000);  
// }

// TEST(BitSequenceTest, Concat) {
//     Bit<uint8_t> a[] = { Bit<uint8_t>(1), Bit<uint8_t>(2) };
//     Bit<uint8_t> b[] = { Bit<uint8_t>(3), Bit<uint8_t>(4), Bit<uint8_t>(5) };
//     BitSequence<uint8_t> seqA(a, 2);
//     BitSequence<uint8_t> seqB(b, 3);
//     Sequence<Bit<uint8_t>>* concat = seqA.Concat(&seqB);
//     EXPECT_EQ(concat->GetLength(), 5);
//     EXPECT_EQ(concat->Get(0).GetValue(), 1);
//     EXPECT_EQ(concat->Get(4).GetValue(), 5);
//     delete concat;
// }

// TEST(BitSequenceTest, Iterator) {
//     Bit<uint8_t> bits[] = { Bit<uint8_t>(1), Bit<uint8_t>(2), Bit<uint8_t>(3) };
//     BitSequence<uint8_t> seq(bits, 3);
    
//     size_t i = 0;
//     for (const auto& val : seq) {
//         if (i == 0) { EXPECT_EQ(val.GetValue(), 1); }
//         else if (i == 1) { EXPECT_EQ(val.GetValue(), 2); }
//         else if (i == 2) { EXPECT_EQ(val.GetValue(), 3); }
//         i++;
//     }
//     EXPECT_EQ(i, 3);
// }

// // ToString

// // ArraySequence ToString
// TEST(ToStringTest, ArraySequence_Empty) {
//     ArraySequence<int> seq;
//     EXPECT_EQ(seq.ToString(), "[]");
// }

// TEST(ToStringTest, ArraySequence_SingleElement) {
//     int arr[] = {42};
//     ArraySequence<int> seq(arr, 1);
//     EXPECT_EQ(seq.ToString(), "[42]");
// }

// TEST(ToStringTest, ArraySequence_MultipleElements) {
//     int arr[] = {1, 2, 3, 4, 5};
//     ArraySequence<int> seq(arr, 5);
//     EXPECT_EQ(seq.ToString(), "[1, 2, 3, 4, 5]");
// }

// TEST(ToStringTest, ArraySequence_AfterModifications) {
//     ArraySequence<int> seq;
//     seq.Append(10)->Append(20)->Prepend(5);
//     EXPECT_EQ(seq.ToString(), "[5, 10, 20]");
// }

// // ListSequence 
// TEST(ToStringTest, ListSequence_Empty) {
//     ListSequence<int> seq;
//     EXPECT_EQ(seq.ToString(), "[]");
// }

// TEST(ToStringTest, ListSequence_SingleElement) {
//     int arr[] = {99};
//     ListSequence<int> seq(arr, 1);
//     EXPECT_EQ(seq.ToString(), "[99]");
// }

// TEST(ToStringTest, ListSequence_MultipleElements) {
//     int arr[] = {10, 20, 30};
//     ListSequence<int> seq(arr, 3);
//     EXPECT_EQ(seq.ToString(), "[10, 20, 30]");
// }

// TEST(ToStringTest, ListSequence_AfterModifications) {
//     ListSequence<int> seq;
//     seq.Append(100)->Append(200)->Prepend(50);
//     EXPECT_EQ(seq.ToString(), "[50, 100, 200]");
// }

// // BitSequence 
// TEST(ToStringTest, BitSequence_Empty) {
//     BitSequence<uint8_t> seq;
//     EXPECT_EQ(seq.ToString(), "[]");
// }

// TEST(ToStringTest, BitSequence_SingleByte) {
//     Bit<uint8_t> bits[] = { Bit<uint8_t>(0b10101010) };
//     BitSequence<uint8_t> seq(bits, 1);
//     EXPECT_EQ(seq.ToString(), "[0, 1, 0, 1, 0, 1, 0, 1]");
// }

// TEST(ToStringTest, BitSequence_MultipleBytes) {
//     Bit<uint8_t> bits[] = { 
//         Bit<uint8_t>(0b00001111), 
//         Bit<uint8_t>(0b11110000) 
//     };
//     BitSequence<uint8_t> seq(bits, 2);
//     EXPECT_EQ(seq.ToString(), "[1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1]");
// }

// TEST(ToStringTest, BitSequence_AfterAppend) {
//     BitSequence<uint8_t> seq;
//     seq.Append(Bit<uint8_t>(0b00000001));
//     seq.Append(Bit<uint8_t>(0b00000010));
//     EXPECT_EQ(seq.ToString(), "[1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0]");
// }

// TEST(ToStringTest, BitSequence_BitwiseOps) {
//     Bit<uint8_t> a[] = { Bit<uint8_t>(0b11001100) };
//     Bit<uint8_t> b[] = { Bit<uint8_t>(0b10101010) };
//     BitSequence<uint8_t> seqA(a, 1);
//     BitSequence<uint8_t> seqB(b, 1);
//     BitSequence<uint8_t> res = seqA & seqB;
//     EXPECT_EQ(res.ToString(), "[0, 0, 0, 1, 0, 0, 0, 1]");
// }
