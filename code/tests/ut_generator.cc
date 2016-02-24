#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <vector>
#include <list>

#include "generator.h"

using namespace testing;

TEST(Generator, Typing){

    auto a = func::sequence(0,1);
    EXPECT_TRUE( (std::is_same< decltype(a)::value_type, int>()) );

    auto b = func::sequence(.0,.1);
    EXPECT_TRUE( (std::is_same< decltype(b)::value_type, double>()) );

    auto c = func::sequence(0u,1u);
    EXPECT_TRUE( (std::is_same< decltype(c)::value_type, unsigned>()) );
}

TEST(Generator, Sequence){

    auto x = func::sequence(0);
    auto it = x.begin();
    for (int i=0; i < 100; ++i){
        ASSERT_EQ (*it, i);
        ++it;
    }
}

TEST(Generator, Sequence2){

    auto x = func::sequence(0,2);
    auto it = x.begin();
    for (int i=0; i < 100; ++i){
        ASSERT_EQ (*it, i*2);
        ++it;
    }
}

TEST(Generator, Sequence_f){

    auto x = func::sequence(0.0, 1.0);
    auto it = x.begin();
    for (int i=0; i < 100; ++i){
        ASSERT_FLOAT_EQ (*it, i);
        ++it;
    }
}

