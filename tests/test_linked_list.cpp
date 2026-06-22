#include <gtest/gtest.h>
#include "core/linked_list.hpp"
#include "exceptions.hpp"

using namespace myLib;

class LinkedListTest : public ::testing::Test {
protected:
    int TEST_ARR[5] = {10, 20, 30, 40, 50};
    size_t TEST_ARR_LEN = 5;
};

TEST_F(LinkedListTest, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), EmptyCollectionException);
    EXPECT_THROW(list.GetLast(), EmptyCollectionException);
}

TEST_F(LinkedListTest, ArrayConstructor) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 50);
    EXPECT_EQ(list.Get(2), 30);
}

TEST_F(LinkedListTest, CopyConstructor) {
    LinkedList<int> original(TEST_ARR, 3);
    LinkedList<int> copy(original);
    
    EXPECT_EQ(copy.Get(0), 10);
    EXPECT_EQ(copy.Get(2), 30);
    
    original.Append(999);
    EXPECT_EQ(original.Get(3), 999);
    EXPECT_THROW(copy.Get(3), IndexOutOfRangeException);
}

TEST_F(LinkedListTest, GetFirstAndGetLast) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 50);
}

TEST_F(LinkedListTest, GetByIndex_Valid) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(4), 50);
}

TEST_F(LinkedListTest, GetByIndex_Invalid) {
    LinkedList<int> list(TEST_ARR, 3);
    EXPECT_THROW(list.Get(3), IndexOutOfRangeException);
}

TEST_F(LinkedListTest, GetSubList_Valid) {
    LinkedList<int> list(TEST_ARR, TEST_ARR_LEN);
    LinkedList<int>* sub = list.GetSubList(1, 3);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->Get(0), 20);
    delete sub;
}

TEST_F(LinkedListTest, Append) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 20);
}

TEST_F(LinkedListTest, Prepend) {
    LinkedList<int> list;
    list.Prepend(20);
    list.Prepend(10);
    EXPECT_EQ(list.GetFirst(), 10);
}

TEST_F(LinkedListTest, InsertAt_Valid) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(30);
    list.InsertAt(20, 1);
    EXPECT_EQ(list.Get(1), 20);
}

TEST_F(LinkedListTest, Concat) {
    int arr1[] = {1, 2};
    int arr2[] = {3, 4};
    LinkedList<int> list1(arr1, 2);
    LinkedList<int> list2(arr2, 2);
    
    LinkedList<int> concat = list1.Concat(list2); 
    
    EXPECT_EQ(concat.Get(0), 1);
    EXPECT_EQ(concat.Get(2), 3);
}