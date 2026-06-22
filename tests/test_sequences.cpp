#include <gtest/gtest.h>
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "exceptions.hpp"

using namespace myLib;

class SequenceTest : public ::testing::Test {
protected:
    int SEQ_TEST_ARR[5] = {1, 2, 3, 4, 5};
    size_t SEQ_TEST_LEN = 5;
};

TEST_F(SequenceTest, ArraySequenceConstructor) {
    ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    EXPECT_EQ(seq.GetLength(), SEQ_TEST_LEN);
    EXPECT_EQ(seq.Get(0), 1);
}

TEST_F(SequenceTest, MutableArraySequence) {
    MutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    seq.Append(999);
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(3), 999);
}

TEST_F(SequenceTest, ImmutableArraySequence) {
    ImmutableArraySequence<int> seq(SEQ_TEST_ARR, 3);
    Sequence<int>* res = seq.Append(999);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(res->GetLength(), 4);
    delete res;
}

TEST_F(SequenceTest, SequenceIterator) {
    ArraySequence<int> seq(SEQ_TEST_ARR, SEQ_TEST_LEN);
    int sum = 0;
    for (const auto& val : seq) {
        sum += val;
    }
    EXPECT_EQ(sum, 15);
}