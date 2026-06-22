#include <gtest/gtest.h>
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"

using namespace myLib;

class DynamicArrayTest : public ::testing::Test {
protected:
    int DA_TEST_ARR[5] = {100, 200, 300, 400, 500};
    size_t DA_TEST_LEN = 5;
};

TEST_F(DynamicArrayTest, ArrayConstructor) {
    DynamicArray<int> arr(DA_TEST_ARR, DA_TEST_LEN);
    EXPECT_EQ(arr.GetSize(), DA_TEST_LEN);
    EXPECT_EQ(arr.Get(0), 100);
}

TEST_F(DynamicArrayTest, SizeConstructor) {
    DynamicArray<int> arr(10);
    EXPECT_EQ(arr.GetSize(), 10);
}

TEST_F(DynamicArrayTest, Resize_Larger) {
    DynamicArray<int> arr(DA_TEST_ARR, 3);
    arr.Resize(5);
    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 100);
}