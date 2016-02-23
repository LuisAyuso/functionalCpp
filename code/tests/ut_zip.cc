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

    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, func::zip_t<std::vector<int>, std::list<float>, std::vector<double>>::value_type>()));
    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, func::zip_t<std::array<int,90>, std::list<float>, std::vector<double>>::value_type>()));

    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, func::zip_t<std::vector<int>, std::vector<float>, std::list<double>>::value_type>()));
    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, func::zip_t<std::vector<int>, std::vector<float>, std::array<double,10>>::value_type>()));
}

TEST(Zip, iterator){
    std::vector<int>   a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};

    auto y   = func::zip_t<std::vector<int>,std::vector<float>>(a,b);
    auto it  = y.begin();
    auto end = y.end();

    EXPECT_EQ (std::get<0>(it.source),  a.begin());
    EXPECT_EQ (std::get<1>(it.source),  b.begin());
    EXPECT_NE (std::get<0>(end.source), a.begin());
    EXPECT_NE (std::get<1>(end.source), b.begin());
    EXPECT_EQ (std::get<0>(end.source), a.end());
    EXPECT_EQ (std::get<1>(end.source), b.end());

    EXPECT_EQ       (it.source, std::make_pair(a.begin(), b.begin()));
    EXPECT_EQ       (*(std::get<0>(it.source)), 1);
    EXPECT_FLOAT_EQ (*(std::get<1>(it.source)), 0.1);

    ++it;
    EXPECT_EQ       (*(std::get<0>(it.source)), 2);
    EXPECT_FLOAT_EQ (*(std::get<1>(it.source)), 0.2);
}

TEST(Zip, two){
    std::vector<int> a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};

    auto y = func::zip_t<std::vector<int>,std::vector<float>>(a,b);
    const auto& z = y.begin();
    EXPECT_EQ(*(std::get<0>(z.source)), *a.begin());
    EXPECT_EQ(*(std::get<1>(z.source)), *b.begin());

    std::vector<std::pair<int,float>> lp;
    std::copy(y.begin(), y.end(), std::back_inserter(lp));

    EXPECT_EQ(lp[0].first, 1);
    EXPECT_EQ(lp[1].first, 2);
    EXPECT_EQ(lp[2].first, 3);
    EXPECT_EQ(lp[3].first, 4);

    EXPECT_FLOAT_EQ(lp[0].second, 0.1);
    EXPECT_FLOAT_EQ(lp[1].second, 0.2);
    EXPECT_FLOAT_EQ(lp[2].second, 0.3);
    EXPECT_FLOAT_EQ(lp[3].second, 0.4);
}

TEST(Zip, three){
    std::vector<int> a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};
    std::list<double> c = {.1,.2,.3,.4};

    auto x = func::zip_t<std::vector<int>,std::vector<float>, std::list<double>>(a,b,c);

    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, decltype(x)::value_type>()));

    std::vector<std::tuple<int,float,double>> lp;
    std::copy(x.begin(), x.end(), std::back_inserter(lp));


    EXPECT_EQ( (std::get<0>(lp[0])), 1);
    EXPECT_EQ( (std::get<0>(lp[1])), 2);
    EXPECT_EQ( (std::get<0>(lp[2])), 3);
    EXPECT_EQ( (std::get<0>(lp[3])), 4);

//    EXPECT_FLOAT_EQ( (std::get<1>(lp[0])), 0.1);
//    EXPECT_FLOAT_EQ( (std::get<1>(lp[1])), 0.2);
//    EXPECT_FLOAT_EQ( (std::get<1>(lp[2])), 0.3);
//    EXPECT_FLOAT_EQ( (std::get<1>(lp[3])), 0.4);
//
//    EXPECT_FLOAT_EQ( (std::get<2>(lp[0])), 0.1);
//    EXPECT_FLOAT_EQ( (std::get<2>(lp[1])), 0.2);
//    EXPECT_FLOAT_EQ( (std::get<2>(lp[2])), 0.3);
//    EXPECT_FLOAT_EQ( (std::get<2>(lp[3])), 0.4);
}
