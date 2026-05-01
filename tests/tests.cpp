#include <gtest/gtest.h>
#include "core/linked_list.hpp"
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"
#include "iterators/ienumerator.hpp"
#include "iterators/array_enumerator.hpp"
#include "iterators/list_enumerator.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/sequence.hpp"

using namespace myLib;

static int TEST_ARR[] = {10, 20, 30, 40, 50};
static int TEST_ARR_LEN = 5;

// LinkedList

TEST(LinkedListTest, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
    EXPECT_THROW(list.GetFirst(), IndexOutOfRangeException);
    EXPECT_THROW(list.GetLast(), IndexOutOfRangeException);
}

TEST(LinkedListTest, ArrayConstructor) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_EQ(list.GetLength(), TEST_ARR_LEN);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 50);
    EXPECT_EQ(list.Get(2), 30);
}

TEST(LinkedListTest, CopyConstructor) {
    LinkedList<int> original(TEST_ARR, 3);
    LinkedList<int> copy(original);
    
    EXPECT_EQ(copy.GetLength(), 3);
    EXPECT_EQ(copy.Get(0), 10);
    EXPECT_EQ(copy.Get(2), 30);
    
    original.Append(999);
    EXPECT_EQ(original.GetLength(), 4);
    EXPECT_EQ(copy.GetLength(), 3);
}

TEST(LinkedListTest, GetFirstAndGetLast) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 50);
}

TEST(LinkedListTest, GetByIndex_Valid) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(4), 50);
    EXPECT_EQ(list.Get(2), 30);
}

TEST(LinkedListTest, GetByIndex_Invalid) {
    LinkedList<int> list(TEST_ARR, 3);
    EXPECT_THROW(list.Get(-1), IndexOutOfRangeException);
    EXPECT_THROW(list.Get(3), IndexOutOfRangeException);
    EXPECT_THROW(list.Get(100), IndexOutOfRangeException);
}

TEST(LinkedListTest, GetSubList_Valid) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    
    LinkedList<int>* sub = list.GetSubList(1, 3);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 20);
    EXPECT_EQ(sub->Get(1), 30);
    EXPECT_EQ(sub->Get(2), 40);
    delete sub;
}

TEST(LinkedListTest, GetSubList_InvalidIndices) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_THROW(list.GetSubList(-1, 2), IndexOutOfRangeException);
    EXPECT_THROW(list.GetSubList(0, 5), IndexOutOfRangeException);
    EXPECT_THROW(list.GetSubList(3, 1), IndexOutOfRangeException);
}

TEST(LinkedListTest, Append) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 30);
}

TEST(LinkedListTest, Prepend) {
    LinkedList<int> list;
    list.Prepend(30);
    list.Prepend(20);
    list.Prepend(10);
    
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 30);
}

TEST(LinkedListTest, InsertAt_Valid) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(30);
    
    list.InsertAt(20, 1);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
    
    list.InsertAt(5, 0);
    EXPECT_EQ(list.Get(0), 5);
    
    list.InsertAt(50, 4);
    EXPECT_EQ(list.GetLast(), 50);
}

TEST(LinkedListTest, InsertAt_Invalid) {
    LinkedList<int> list;
    list.Append(10);
    
    EXPECT_THROW(list.InsertAt(99, -1), IndexOutOfRangeException);
    EXPECT_THROW(list.InsertAt(99, 2), IndexOutOfRangeException);
}

TEST(LinkedListTest, Concat) {
    int arr1[] = {1, 2};
    int arr2[] = {3, 4, 5};
    LinkedList<int> list1(arr1, 2);
    LinkedList<int> list2(arr2, 3);
    
    LinkedList<int>* concat = list1.Concat(&list2);
    ASSERT_NE(concat, nullptr);
    EXPECT_EQ(concat->GetLength(), 5);
    EXPECT_EQ(concat->Get(0), 1);
    EXPECT_EQ(concat->Get(2), 3);
    EXPECT_EQ(concat->Get(4), 5);
    
    EXPECT_EQ(list1.GetLength(), 2);
    EXPECT_EQ(list2.GetLength(), 3);
    
    delete concat;
}

TEST(LinkedListTest, Concat_EmptyList) {
    LinkedList<int> list;
    int arr[] = {1, 2};
    LinkedList<int> other(arr, 2);
    
    LinkedList<int>* res1 = list.Concat(&other);
    EXPECT_EQ(res1->GetLength(), 2);
    EXPECT_EQ(res1->Get(0), 1);
    delete res1;
    
    LinkedList<int>* res2 = other.Concat(&list);
    EXPECT_EQ(res2->GetLength(), 2);
    EXPECT_EQ(res2->GetLast(), 2);
    delete res2;
}

// DynamicArray

static int DA_TEST_ARR[] = {100, 200, 300, 400, 500};
static int DA_TEST_LEN = 5;

TEST(DynamicArrayTest, ArrayConstructor) {
    DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
    EXPECT_EQ(arr.GetSize(), DA_TEST_LEN);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(2), 300);
    EXPECT_EQ(arr.Get(4), 500);
}

TEST(DynamicArrayTest, SizeConstructor) {
    DynamicArray<int> arr(10);
    EXPECT_EQ(arr.GetSize(), 10);
}

TEST(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> original(DA_TEST_ARR, 3);
    DynamicArray<int> copy(original);
    
    EXPECT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(copy.Get(0), 100);
    EXPECT_EQ(copy.Get(2), 300);
    
    original.Set(0, 999);
    EXPECT_EQ(original.Get(0), 999);
    EXPECT_EQ(copy.Get(0), 100);
}

TEST(DynamicArrayTest, Get_Valid) {
    DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(4), 500);
    EXPECT_EQ(arr.Get(2), 300);
}

TEST(DynamicArrayTest, Get_Invalid) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    EXPECT_THROW(arr.Get(-1), IndexOutOfRangeException);
    EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
    EXPECT_THROW(arr.Get(100), IndexOutOfRangeException);
}

TEST(DynamicArrayTest, GetSize) {
    DynamicArray<int> arr1(0);
    EXPECT_EQ(arr1.GetSize(), 0);
    
    DynamicArray<int> arr2(DA_TEST_ARR, DA_TEST_LEN);
    EXPECT_EQ(arr2.GetSize(), DA_TEST_LEN);
}

TEST(DynamicArrayTest, Set_Valid) {
    DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
    arr.Set(0, 1000);
    arr.Set(2, 3000);
    arr.Set(4, 5000);
    
    EXPECT_EQ(arr.Get(0), 1000);
    EXPECT_EQ(arr.Get(2), 3000);
    EXPECT_EQ(arr.Get(4), 5000);
}

TEST(DynamicArrayTest, Set_Invalid) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    EXPECT_THROW(arr.Set(-1, 100), IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(3, 100), IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(100, 100), IndexOutOfRangeException);
}

TEST(DynamicArrayTest, Resize_Larger) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    arr.Resize(5);
    
    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(1), 200);
    EXPECT_EQ(arr.Get(2), 300);
    EXPECT_EQ(arr.Get(3), 0);
    EXPECT_EQ(arr.Get(4), 0);
}

TEST(DynamicArrayTest, Resize_Smaller) {
    DynamicArray<int> arr(DA_TEST_ARR, 5);
    arr.Resize(3);
    
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(1), 200);
    EXPECT_EQ(arr.Get(2), 300);
    EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
    EXPECT_THROW(arr.Get(4), IndexOutOfRangeException);
}

TEST(DynamicArrayTest, Resize_Same) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    arr.Resize(3);
    
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(2), 300);
}

TEST(DynamicArrayTest, Resize_Zero) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    arr.Resize(0);
    
    EXPECT_EQ(arr.GetSize(), 0);
    EXPECT_THROW(arr.Get(0), IndexOutOfRangeException);
}

TEST(DynamicArrayTest, Resize_Invalid) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    EXPECT_THROW(arr.Resize(-1), IndexOutOfRangeException);
    EXPECT_THROW(arr.Resize(-100), IndexOutOfRangeException);
}

TEST(DynamicArrayTest, Resize_PreservesData) {
    int arr[] = {1, 2, 3, 4, 5};
    DynamicArray<int> da(arr, 5);
    
    da.Resize(10);
    da.Set(5, 60);
    da.Set(9, 100);
    
    EXPECT_EQ(da.Get(0), 1);
    EXPECT_EQ(da.Get(4), 5);
    EXPECT_EQ(da.Get(5), 60);
    EXPECT_EQ(da.Get(9), 100);
    EXPECT_EQ(da.GetSize(), 10);
}

TEST(DynamicArrayTest, EmptyArray) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetSize(), 0);
    EXPECT_THROW(arr.Get(0), IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(0, 10), IndexOutOfRangeException);
}

// SEQUENCE

static int SEQ_TEST_ARR[] = {1, 2, 3, 4, 5};
static int SEQ_TEST_LEN = 5;

// ArraySequence

TEST(ArraySequenceTest, ArrayConstructor) {
    ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    EXPECT_EQ(seq.GetLength(), SEQ_TEST_LEN);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(4), 5);
}

TEST(ArraySequenceTest, GetFirstLast) {
    ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 5);
}

TEST(ArraySequenceTest, GetSubsequence) {
    ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(2), 4);
    delete sub;
}

TEST(ArraySequenceTest, AppendPrependInsertAt_MutableBehavior) {
    ArraySequence<int> seq;
    seq.Append(2);
    seq.Append(3);
    seq.Prepend(1);
    seq.InsertAt(4, 3);
    
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(ArraySequenceTest, Concat_ReturnsNewSequence) {
    int arr1[] = {1, 2};
    int arr2[] = {3, 4, 5};
    ArraySequence<int> seq1(arr1, 2);
    ArraySequence<int> seq2(arr2, 3);
    
    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_NE(static_cast<void*>(result), static_cast<void*>(&seq1));
    EXPECT_EQ(seq1.GetLength(), 2);
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(4), 5);
    
    delete result;
}

// Mutable

TEST(MutableArraySequenceTest, Append_ModifiesOriginal) {
    MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq.Append(999);
    
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(3), 999);
    EXPECT_EQ(static_cast<void*>(result), static_cast<void*>(&seq));
}

TEST(MutableArraySequenceTest, Prepend_ModifiesOriginal) {
    MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq.Prepend(0);
    
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 0);
    EXPECT_EQ(static_cast<void*>(result), static_cast<void*>(&seq));
}

TEST(MutableArraySequenceTest, InsertAt_ModifiesOriginal) {
    MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq.InsertAt(99, 1);
    
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(1), 99);
    EXPECT_EQ(static_cast<void*>(result), static_cast<void*>(&seq));
}

TEST(MutableArraySequenceTest, ChainOperations) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(MutableArraySequenceTest, InsertAt_InvalidIndex) {
    MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    EXPECT_THROW(seq.InsertAt(99, -1), IndexOutOfRangeException);
    EXPECT_THROW(seq.InsertAt(99, 4), IndexOutOfRangeException);
}

// Immutable

TEST(ImmutableArraySequenceTest, Append_CreatesNewSequence) {
    ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq.Append(999);
    
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(2), 3);
    
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(3), 999);
    EXPECT_NE(static_cast<void*>(result), static_cast<void*>(&seq));
    
    delete result;
}

TEST(ImmutableArraySequenceTest, Prepend_CreatesNewSequence) {
    ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq.Prepend(0);
    
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 0);
    
    delete result;
}

TEST(ImmutableArraySequenceTest, InsertAt_CreatesNewSequence) {
    ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq.InsertAt(99, 1);
    
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(1), 2);
    
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(1), 99);
    
    delete result;
}

TEST(ImmutableArraySequenceTest, Concat_CreatesNewSequence) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    ImmutableArraySequence<int> seq1(arr1, 3);
    ImmutableArraySequence<int> seq2(arr2, 2);
    
    Sequence<int>* result = seq1.Concat(&seq2);
    
    EXPECT_EQ(seq1.GetLength(), 3);
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(4), 5);
    
    delete result;
}

// Many Immutables
TEST(ImmutableArraySequenceTest, ChainOperations_Safe) {
    ImmutableArraySequence<int> seq;
    Sequence<int>* s1 = seq.Append(1);
    Sequence<int>* s2 = s1->Append(2);
    Sequence<int>* s3 = s2->Append(3);
    
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(s1->GetLength(), 1);
    EXPECT_EQ(s2->GetLength(), 2);
    EXPECT_EQ(s3->GetLength(), 3);
    EXPECT_EQ(s3->Get(2), 3);
    
    delete s1;
    delete s2;
    delete s3;
}

TEST(ImmutableArraySequenceTest, EmptySequence_Append) {
    ImmutableArraySequence<int> seq;
    Sequence<int>* result = seq.Append(1);
    
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 1);
    
    delete result;
}

TEST(ImmutableArraySequenceTest, InsertAt_InvalidIndex) {
    ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    EXPECT_THROW(seq.InsertAt(99, -1), IndexOutOfRangeException);
    EXPECT_THROW(seq.InsertAt(99, 4), IndexOutOfRangeException);
}

// Mutable and Immutable

TEST(MutableVsImmutableTest, Append_Behavior) {
    MutableArraySequence<int> mutableSeq(SEQ_TEST_ARR, 3);
    Sequence<int>* mutableResult = mutableSeq.Append(999);
    
    ImmutableArraySequence<int> immutableSeq(SEQ_TEST_ARR, 3);
    Sequence<int>* immutableResult = immutableSeq.Append(999);
    
    EXPECT_EQ(mutableSeq.GetLength(), 4);
    EXPECT_EQ(static_cast<void*>(mutableResult), static_cast<void*>(&mutableSeq));
    
    EXPECT_EQ(immutableSeq.GetLength(), 3);
    EXPECT_NE(static_cast<void*>(immutableResult), static_cast<void*>(&immutableSeq));
    
    delete immutableResult;
}

TEST(MutableVsImmutableTest, Prepend_Behavior) {
    MutableArraySequence<int> mutableSeq;
    mutableSeq.Prepend(1);
    mutableSeq.Prepend(2);
    
    ImmutableArraySequence<int> immutableSeq;
    Sequence<int>* s1 = immutableSeq.Prepend(1);
    Sequence<int>* s2 = s1->Prepend(2);
    
    EXPECT_EQ(mutableSeq.GetLength(), 2);
    EXPECT_EQ(mutableSeq.Get(0), 2);
    
    EXPECT_EQ(immutableSeq.GetLength(), 0);
    EXPECT_EQ(s2->GetLength(), 2);
    EXPECT_EQ(s2->Get(0), 2);
    
    delete s1;
    delete s2;
}

TEST(MutableVsImmutableTest, InsertAt_Behavior) {
    MutableArraySequence<int> mutableSeq(SEQ_TEST_ARR, 3);
    mutableSeq.InsertAt(99, 1);
    
    ImmutableArraySequence<int> immutableSeq(SEQ_TEST_ARR, 3);
    Sequence<int>* result = immutableSeq.InsertAt(99, 1);
    
    EXPECT_EQ(mutableSeq.Get(1), 99);
    EXPECT_EQ(immutableSeq.Get(1), 2);
    EXPECT_EQ(result->Get(1), 99);
    
    delete result;
}

// Полиморфизм

TEST(PolymorphismTest, MutableArraySequenceViaBasePointer) {
    Sequence<int>* seq = new MutableArraySequence<int>(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq->Append(999);
    
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(3), 999);
    EXPECT_EQ(static_cast<void*>(result), static_cast<void*>(seq));
    
    delete seq;
}

TEST(PolymorphismTest, ImmutableArraySequenceViaBasePointer) {
    Sequence<int>* seq = new ImmutableArraySequence<int>(SEQ_TEST_ARR, 3);
    Sequence<int>* result = seq->Append(999);
    
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_NE(static_cast<void*>(result), static_cast<void*>(seq));
    
    delete seq;
    delete result;
}

// Exceptions
TEST(SequenceExceptionsTest, Get_InvalidIndex) {
    ArraySequence<int> seq(SEQ_TEST_ARR, 3);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRangeException);
    EXPECT_THROW(seq.Get(3), IndexOutOfRangeException);
}

TEST(SequenceExceptionsTest, GetSubsequence_Invalid) {
    ListSequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    EXPECT_THROW(seq.GetSubsequence(-1, 2), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetSubsequence(0, 10), IndexOutOfRangeException);
}

TEST(SequenceExceptionsTest, EmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRangeException);
    EXPECT_THROW(seq.Get(0), IndexOutOfRangeException);
}

TEST(IteratorTest, ArraySequence_Traversal) {
    ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    IEnumerator<int>* it = seq.GetEnumerator();
    
    int expected[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < SEQ_TEST_LEN; i++) {
        EXPECT_TRUE(it->MoveNext());
        EXPECT_EQ(it->Current(), expected[i]);
    }
    EXPECT_FALSE(it->MoveNext());
    
    it->Reset();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->Current(), 1);
    
    delete it;
}

// Iterator
TEST(IteratorTest, ListSequence_Traversal) {
    ListSequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    IEnumerator<int>* it = seq.GetEnumerator();
    
    int sum = 0;
    while (it->MoveNext()) {
        sum += it->Current();
    }
    EXPECT_EQ(sum, 15);
    EXPECT_FALSE(it->MoveNext());
    
    delete it;
}

TEST(IteratorTest, EmptySequence) {
    ArraySequence<int> seq;
    IEnumerator<int>* it = seq.GetEnumerator();
    EXPECT_FALSE(it->MoveNext());
    delete it;
}

TEST(IteratorTest, Reset_MultipleTimes) {
    ListSequence<int> seq(SEQ_TEST_ARR, 3);
    IEnumerator<int>* it = seq.GetEnumerator();
    
    it->MoveNext(); 
    it->MoveNext();
    EXPECT_EQ(it->Current(), 2);
    
    it->Reset();
    it->MoveNext();
    EXPECT_EQ(it->Current(), 1);
    
    delete it;
}