#include <gtest/gtest.h>
#include "option.hpp"
#include "exceptions.hpp"
#include <string>

using namespace myLib;

namespace {
    myLib::Option<std::string> intToString(int x) {
        return myLib::Option<std::string>(std::to_string(x));
    }
    myLib::Option<int> getFallback() {
        return myLib::Option<int>(100); 
    }
}

TEST(OptionTest, NullOptConstructor) {
    Option<int> opt(nullopt);
    EXPECT_TRUE(opt.IsNone());
    EXPECT_FALSE(opt.HasValue());
}

TEST(OptionTest, MonadicAndThen) {
    Option<int> opt(42);
    auto res = std::move(opt).and_then(intToString);
    EXPECT_TRUE(res.HasValue());
    EXPECT_EQ(res.GetValue(), "42");
}

TEST(OptionTest, MonadicOrElse) {
    Option<int> opt(nullopt);
    auto res = std::move(opt).or_else(getFallback);
    EXPECT_TRUE(res.HasValue());
    EXPECT_EQ(res.GetValue(), 100);
}

TEST(OptionTest, IteratorRange) {
    Option<int> some(100);
    int count = 0;
    int last_val = 0;
    
    for (int x : some) {
        count++;
        last_val = x;
    }
    
    EXPECT_EQ(count, 1);
    EXPECT_EQ(last_val, 100);
}

TEST(OptionTest, EmptyIterator) {
    Option<int> none(nullopt);
    int count = 0;
    
    for (int x : none) {
        count++;
    }
    EXPECT_EQ(count, 0);
}