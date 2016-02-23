#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <list>
#include <array>

#include <random>

#include "zip.h"

using namespace testing;

TEST(Zip, traits){
    EXPECT_TRUE((std::is_same<int, func::zip_t<std::vector<int>>::value_type>::value));
    EXPECT_TRUE((std::is_same<std::pair<int,float>, func::zip_t<std::vector<int>, std::vector<float>>::value_type>()));
    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, func::zip_t<std::vector<int>, std::vector<float>, std::vector<double>>::value_type>()));
}

TEST(Zip, iterator){
    std::vector<int> a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};

    auto y = func::zip_t<std::vector<int>,std::vector<float>>(a,b);
    auto z = y.begin();
    auto zz = y.end();
    EXPECT_EQ(std::get<0>(z.source), a.begin());
    EXPECT_EQ(std::get<1>(z.source), b.begin());
    EXPECT_EQ(std::get<0>(zz.source), a.end());
    EXPECT_EQ(std::get<1>(zz.source), b.end());

    EXPECT_EQ(z.source, std::make_pair(a.begin(), b.begin()));

    EXPECT_EQ(*(std::get<0>(z.source)), 1);
    EXPECT_FLOAT_EQ(*(std::get<1>(z.source)), 0.1);

    ++z;
    EXPECT_EQ(*(std::get<0>(z.source)), 2);
    EXPECT_FLOAT_EQ(*(std::get<1>(z.source)), 0.2);

}

TEST(Zip, iterator1){
    std::vector<int> a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};

    auto y = func::zip_t<std::vector<int>,std::vector<float>>(a,b);
    const auto& z = y.begin();
    EXPECT_EQ(*(std::get<0>(z.source)), *a.begin());
    EXPECT_EQ(*(std::get<1>(z.source)), *b.begin());

    std::vector<std::pair<int,float>> lp;
    std::copy(y.begin(), y.end(), std::back_inserter(lp));
    EXPECT_EQ(lp[0].first, 1);
    EXPECT_EQ(lp[0].second, 0.1);
    EXPECT_EQ(lp[1].first, 2);
    EXPECT_EQ(lp[1].second, 0.2);
    EXPECT_EQ(lp[2].first, 3);
    EXPECT_EQ(lp[2].second, 0.3);
    EXPECT_EQ(lp[3].first, 4);
    EXPECT_EQ(lp[3].second, 0.4);

}



