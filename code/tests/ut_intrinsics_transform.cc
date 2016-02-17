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
        
    std::function< int(int)> func = [] (int i) { return i; };
    std::vector<int> vec = {1,2,3,4,5,6};

    transform_t<int,int,std::vector<int>,detail::Reference_storage>  x(func, detail::transform_store_t<std::vector<int>, detail::Reference_storage>(vec));

    std::vector<int> res (x.begin(), x.end());
    EXPECT_THAT(res, ElementsAre(1,2,3,4,5,6));
}

TEST(Transform_t, ctor_val){
        
    std::function< int(int)> func = [] (int i) { return i; };
    std::vector<int> vec = {2,2,3,3,5,6};

    transform_t<int,int,std::vector<int>,detail::Value_storage>  x(func, detail::transform_store_t<std::vector<int>, detail::Value_storage>(std::move(vec)));

    std::vector<int> res (x.begin(), x.end());
    EXPECT_THAT(res, ElementsAre(2,2,3,3,5,6));
}

TEST(Transform_t, chain_ref){

    std::function< int(int)> func1 = [] (int i) { return 1+i; };
    std::function< int(int)> func2 = [] (int i) { return 2*i; };
    std::vector<int> vec = {1,1,2,2};

    transform_t<int,int,std::vector<int>,detail::Reference_storage>  x(func1, detail::transform_store_t<std::vector<int>, detail::Reference_storage>(vec));
    using in_t = decltype (x);

    transform_t<int,int, in_t, detail::Reference_storage>  y(func2, x);

    std::vector<int> res (y.begin(), y.end());
    EXPECT_THAT(res, ElementsAre(4,4,6,6));
}

TEST(Transform_t, chain_val){

    std::function< int(int)> func1 = [] (int i) { return 1+i; };
    std::function< int(int)> func2 = [] (int i) { return 2*i; };

    using inside_t = transform_t<int,int,std::vector<int>,detail::Value_storage>;
        
    transform_t<int,int, inside_t, detail::Value_storage>  x(func2, inside_t(func1, detail::transform_store_t<std::vector<int>, detail::Value_storage>(std::vector<int>{{2,2,1,1}})));

    std::vector<int> res (x.begin(), x.end());
    EXPECT_THAT(res, ElementsAre(6,6,4,4));
}
