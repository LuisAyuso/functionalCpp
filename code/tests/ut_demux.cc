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

#include "demux.h"

using namespace testing;

TEST(Demux, empty){

    std::vector<int> v;
    auto x = func::demux([](auto& it) -> std::vector<char>{
        return {0};
    }, v);

    ASSERT_TRUE ((std::is_same<decltype(x)::iterator::local_collection, std::vector<char>>()));
    std::vector<char> res(x.begin(), x.end());
    ASSERT_TRUE(res.empty());
}

TEST(Demux, digits){

    {
        std::vector<int> v;
        auto x = func::demux([](auto v) -> std::vector<char>{
            std::vector<char> res;
            return res;
        }, v);

        ASSERT_TRUE(v.begin() == v.end());
        ASSERT_TRUE(x.begin() == x.end());
        ASSERT_FALSE(x.begin() != x.end());
        std::vector<char> res(x.begin(), x.end());
        ASSERT_TRUE(res.empty());
    }
    std::cout << " -------------- " << std::endl;
    {
        std::vector<int> v{1};
        auto x = func::demux([](auto v) -> std::vector<char>{
            std::vector<char> res;
            std::cout << "compute " << v << std::endl;
            if (v >1000) res.push_back('d');
            if (v >100) res.push_back('c');
            if (v >10) res.push_back('b');
            if ( v >0) res.push_back('a');
            std::cout << "res:" << res.size() << std::endl;
            return res;
        }, v);

        std::vector<char> res(x.begin(), x.end());
        ASSERT_FALSE(res.empty());
        ASSERT_EQ(1, res.size());
    }
    std::cout << " -------------- " << std::endl;
    {
        std::vector<int> v{1,23,456,7890};
        auto x = func::demux([](auto v) -> std::vector<char>{
            std::vector<char> res;
            std::cout << "compute " << v << std::endl;
            if (v >1000) res.push_back('d');
            if (v >100) res.push_back('c');
            if (v >10) res.push_back('b');
            if ( v >0) res.push_back('a');
            std::cout << "res:" << res.size() << std::endl;
            return res;
        }, v);

        std::vector<char> res(x.begin(), x.end());
        ASSERT_FALSE(res.empty());
        ASSERT_EQ(10, res.size());
        EXPECT_THAT(res, ElementsAre('a','b','a','c','b','a','d','c','b','a'));
    }
}
