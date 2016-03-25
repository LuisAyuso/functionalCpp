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
#include <string>

#include <random>

#include "mux.h"

using namespace testing;

TEST(Mux, lambda1){

        // lambda in variable.
        std::vector<int> v;
        for (int i =0;i<10;i++) v.push_back(i);

        using it_t = std::vector<int>::iterator;

        auto f = [&]( it_t& it, const it_t& end) -> int {
            auto v = *it;
            it++;
            if (it == end ) return v;
            v = *it;
            it ++;
            if (it == end ) return v;
            return *it;
        };

        auto x = func::mux(f, v);

        std::vector<int> res (x.begin(), x.end());
        EXPECT_EQ(5, res.size());
        // the original iterator points to 0, therefore is 2, 4, 6,8 and then only half step
        EXPECT_THAT(res, ElementsAre(2,4,6,8,9));
}

TEST(Mux, empty){

    auto f = [](auto& it, const auto& end) -> int{
        int count = 0;
        while (it != end) {
            ++count;
            ++it;
            if (count == 2) return count;
        }
        return count;
    };

    {
        std::vector<int> v;
        using it_t = std::vector<int>::iterator;

        auto x = func::mux([](it_t& it, const it_t& end) -> int{
                return -1;
        }, v);

        std::vector<int> res(x.begin(), x.end());
        EXPECT_TRUE(res.empty());
    }

    {
        std::vector<int> v{ 1,2};
        using it_t = std::vector<int>::iterator;

        auto x = func::mux(f, v);

        std::vector<int> res(x.begin(), x.end());
        ASSERT_FALSE(res.empty());
        EXPECT_EQ(res.size(), 1);
        EXPECT_EQ(res[0], 2);
    }

    {
        std::vector<int> v{ 1,2,3};
        using it_t = std::vector<int>::iterator;

        auto x = func::mux(f, v);

        std::vector<int> res(x.begin(), x.end());
        ASSERT_EQ(res.size(), 2); 
        EXPECT_EQ(2, res[0] );
        EXPECT_EQ(1, res[1] );
    }
}

TEST(Mux, vectors){

        std::string cad (" hello\n new line \n adios.");

        using it_t = std::string::iterator;

        auto f = [&]( it_t& it, const it_t& end) -> std::string {
            std::string ret;
            while (it != end && *it != '\n') {
                ret += *it;
                ++it;
            }
            if (it != end) ++it;
            return ret;
        };

        auto x = func::mux(f, cad);

        std::vector<std::string> res (x.begin(), x.end());
        EXPECT_EQ(3, res.size());
        EXPECT_EQ(" hello", res[0]);
        EXPECT_EQ(" new line ", res[1]);
        EXPECT_EQ(" adios.", res[2]);
}
