/**
	FunctionalCpp,  A header only library for chainable functional operations
	in C++ collections
    Copyright (C) 2016 Luis F. Ayuso & Stefan Moosbrugger

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

TEST(Generator, Limited_Sequence){

    auto x = func::sequence(0.0, 1.0, 10);
    auto it = x.begin();
    for (int i=0; i < 10; ++i){
        ASSERT_FLOAT_EQ (*it, i);
        ++it;
    }
    EXPECT_EQ(it.count, 0);
    EXPECT_EQ(it, x.end());
    EXPECT_FLOAT_EQ(it.v, 10.0);
}

TEST(Generator, Limited_Sequence_Methods){

    auto x = func::sequence(0, 1, 10);
    auto it = x.begin();

    EXPECT_EQ(it[1], 1);
    EXPECT_EQ(it[5], 5);
    EXPECT_EQ(*(it+3), 3);
    it++;
    it++;
    EXPECT_EQ(*(it), 2);
    EXPECT_EQ(*(it-1), 1);
}
