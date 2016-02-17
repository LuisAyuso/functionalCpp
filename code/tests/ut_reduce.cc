#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <functional>
#include <vector>
#include <map>
#include <list>
#include <array>

#include "reduce.h"

using namespace testing;

TEST(ReduceTest, lambda1){
    std::vector<int> v {{1,1,1,1,1,1}};
    auto res = func::reduce([](int a, int b) -> int { return a+b; }, v, 0);
    EXPECT_EQ(res, 6);
}

TEST(ReduceTest, lambda2){
    std::vector<int> v {{1,1,1,1,1,1}};
    auto f = [](int a, int b) -> int { return a+b; };
    auto res = func::reduce(f, v, 0);
    EXPECT_EQ(res, 6);
}

TEST(ReduceTest, func1){
    std::vector<int> v {{1,1,1,1,1,1}};
    std::function<int(int, int)> f = [](int a, int b) -> int { return a+b; };
    auto res = func::reduce(f, v, 0);
    EXPECT_EQ(res, 6);
}

TEST(ReduceTest, otherType){

    using mytype = std::pair<int, float>;

    std::vector<mytype> v {{ {1,0.1},{1,0.1},{1,0.1} }};

    std::function<mytype(mytype,mytype)> f = [](mytype a, mytype b) -> mytype { 
        return std::make_pair(a.first + b.first, a.second + b.second);
    };
    auto res = func::reduce(f, v, std::make_pair(0, 0.0));
    EXPECT_EQ(res.first, 3);
    EXPECT_FLOAT_EQ(res.second, 0.3);
}
