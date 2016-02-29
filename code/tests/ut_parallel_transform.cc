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
#include <functional>
#include <vector>
#include <map>
#include <list>
#include <array>

#include <random>


#include "transform.h"

using namespace testing;

TEST(ParallelTransform, Simple) {
    std::vector<int> v;
    for (int i =0;i<10;i++) v.push_back(i);
    std::function<int (int)> f = [](int a) -> int { return a+1; };
    std::function<int (int)> g = [](int a) -> int { return a*2; };

    auto x = func::transform(f, v);
    ASSERT_TRUE(x.begin().is_parallel_iterator);
    EXPECT_EQ(x.end()-x.begin(), 10);
    EXPECT_EQ(x.begin()+10, x.end());
    EXPECT_EQ((x.end()-(x.begin()+8)), 2);
    EXPECT_EQ(x.begin()[1], 2);
    EXPECT_EQ(*(x.end()-1), 10);

    auto y = func::transform(g, x);
    ASSERT_TRUE(y.begin().is_parallel_iterator);
    EXPECT_EQ(y.end()-y.begin(), 10);
    EXPECT_EQ(y.begin()+10, y.end());
    EXPECT_EQ((y.end()-(y.begin()+8)), 2);
    EXPECT_EQ(y.begin()[1], 4);
    EXPECT_EQ(*(y.end()-1), 20);
}
