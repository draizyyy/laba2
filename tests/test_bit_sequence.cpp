#include <gtest/gtest.h>
#include "sequences/bit_sequence.hpp"
#include "exceptions.hpp"

using namespace myLib;

namespace {
    int Square(int x) { return x * x; }
    bool IsEven(int x) { return x % 2 == 0; }
    
    Bit<uint8_t> InvertLowNibble(Bit<uint8_t> b) { 
        return Bit<uint8_t>(b.GetValue() ^ 0x0F); 
    }
}

TEST(BitTest, ConstructorAndValue) {
    Bit<uint8_t> b(0b10101010);
    EXPECT_EQ(b.GetValue(), 0b10101010);
}

TEST(BitSequenceTest, AppendPrepend) {
    BitSequence<uint8_t> seq;
    seq.Append(Bit<uint8_t>(0b1010));
    EXPECT_EQ(seq.GetLength(), 1);
}