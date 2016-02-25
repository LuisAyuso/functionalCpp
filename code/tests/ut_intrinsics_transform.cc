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
using namespace func;

TEST(Transform_t, ctor_ref){

    using F = std::function< int(int)>;
        
    F func = [] (int i) { return i; };
    std::vector<int> vec = {1,2,3,4,5,6};

    transform_t<F ,std::vector<int>,detail::Reference_storage>  x(func, detail::chaineable_store_t<std::vector<int>, detail::Reference_storage>(vec));

    std::vector<int> res (x.begin(), x.end());
    EXPECT_THAT(res, ElementsAre(1,2,3,4,5,6));
}

TEST(Transform_t, ctor_val){
        
    using F = std::function< int(int)>;
    F func = [] (int i) { return i; };
    std::vector<int> vec = {2,2,3,3,5,6};

    transform_t<F,std::vector<int>,detail::Value_storage>  x(func, detail::chaineable_store_t<std::vector<int>, detail::Value_storage>(std::move(vec)));

    std::vector<int> res (x.begin(), x.end());
    EXPECT_THAT(res, ElementsAre(2,2,3,3,5,6));
}

TEST(Transform_t, chain_ref){

    using F = std::function< int(int)>;

    F func1 = [] (int i) { return 1+i; };
    F func2 = [] (int i) { return 2*i; };
    std::vector<int> vec = {1,1,2,2};

    transform_t<F,std::vector<int>,detail::Reference_storage>  x(func1, detail::chaineable_store_t<std::vector<int>, detail::Reference_storage>(vec));
    using in_t = decltype (x);

    transform_t<F, in_t, detail::Reference_storage>  y(func2, x);

    std::vector<int> res (y.begin(), y.end());
    EXPECT_THAT(res, ElementsAre(4,4,6,6));
}

TEST(Transform_t, chain_val){

    using F = std::function< int(int)>;
    std::function< int(int)> func1 = [] (int i) { return 1+i; };
    std::function< int(int)> func2 = [] (int i) { return 2*i; };

    using inside_t = transform_t<F,std::vector<int>,detail::Value_storage>;
        
    transform_t<F, inside_t, detail::Value_storage>  x(func2, inside_t(func1, detail::chaineable_store_t<std::vector<int>, detail::Value_storage>(std::vector<int>{{2,2,1,1}})));

    std::vector<int> res (x.begin(), x.end());
    EXPECT_THAT(res, ElementsAre(6,6,4,4));
}
