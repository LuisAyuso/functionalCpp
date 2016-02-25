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


#include "filter.h"

using namespace testing;

TEST(Filter, lambda1){
        // lambda in variable.
        
        std::vector<int> v;
        for (int i =0;i<10;i++) v.push_back(i);
        std::function<bool (int)> f = [](int a) { return a>4; };
        
        auto x = func::filter(f, v);

        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(5,6,7,8,9));
    }

TEST(Filter, inline_lambda){
        // inline lambda
        std::vector<int> v;
        for (int i =0;i<10;i++) v.push_back(i);
                
        auto f = [](int a) -> bool{ return a%2; };
        
        auto x = func::filter(f, v);
        
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(1,3,5,7,9));
    }
   
TEST(Filter, x_value){
        // X value
        std::function<int (int)> f =[](int x) -> bool {return x%2;};
        auto x = func::filter(f, std::vector<int>({1,2,3,4,5}));
        
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(1,3,5));
    }
    
TEST(Filter, std_function){
        // function type
        std::map<int, int> m;
        m[1] = 1;
        m[2] = 1;
        std::function< bool (std::pair<const int,int>)> f =[](std::pair<int, int> x) -> bool {return x.first == x.second;};
        auto x = func::filter(f,m);
    
    
        std::vector<std::pair<const int, int>> res (x.begin(), x.end());
        EXPECT_EQ(res.size(), 1);
    }
