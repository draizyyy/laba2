#include <gtest/gtest.h>
#include "core/linked_list.hpp"
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"

using namespace myLib;

static int TEST_ARR[] = {10, 20, 30, 40, 50};
static int TEST_ARR_LEN = 5;

// LINKED LIST TESTS

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

// DYNAMIC ARRAY TESTS

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