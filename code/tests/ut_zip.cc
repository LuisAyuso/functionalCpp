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

#include "zip.h"

using namespace testing;

template <typename T>
void error_type(const T& t, std::enable_if<false>* null = nullptr){
    typename T::x value;
}

using vec_int = std::vector<int>;
using vec_float = std::vector<float>;
using list_char = std::list<char>;
using vec_string = std::list<std::string>;

TEST(Zip, traits){

    using namespace func;
    using namespace func::detail;

    using test1 = std::tuple<ref_t<vec_int>, ref_t<vec_float>>;
    EXPECT_TRUE((std::is_same< std::pair<int, float>, unstore_t<test1>>() ));

    using test2 = std::tuple<ref_t<vec_float>, ref_t<vec_int>>;
    EXPECT_TRUE((std::is_same< std::pair<float, int>, unstore_t<test2>>() ));

    using test3 = std::tuple<ref_t<vec_float>, ref_t<vec_int>, val_t<list_char>>;
    EXPECT_TRUE((std::is_same< std::tuple<float, int, char>, unstore_t<test3>>() ));

    using test4 = std::tuple<ref_t<vec_float>, val_t<vec_string>, ref_t<vec_int>, val_t<list_char>>;
    EXPECT_TRUE((std::is_same< std::tuple<float, std::string, int, char>, unstore_t<test4>>() ));
}

TEST(Zip, helper){
    vec_int a;
    vec_float b;
    list_char c;
    vec_string d;

    auto x = func::zip(a,b);
    EXPECT_TRUE((std::is_same< std::pair<int, float>, decltype(x)::value_type>() ));

    auto y = func::zip(a,c);
    EXPECT_TRUE((std::is_same< std::pair<int, char>, decltype(y)::value_type>() ));

    auto z = func::zip(d,c);
    EXPECT_TRUE((std::is_same< std::pair<std::string, char>, decltype(z)::value_type>() ));
}

TEST(Zip, capture_value){

    using namespace func;
    using namespace func::detail;

    vec_int a;
    vec_float b;
    list_char c;
    vec_string d;

    auto x = func::zip(vec_int(100),a);
    EXPECT_TRUE((std::is_same< std::pair<int, int>, decltype(x)::value_type>() ));
    EXPECT_EQ(std::get<1>(x.begin().source), a.begin());
    EXPECT_EQ(std::get<1>(x.end().source), a.end());

    auto y = func::zip(a,vec_float(19));
    EXPECT_TRUE((std::is_same< std::pair<int, float>, decltype(y)::value_type>() ));

    auto z = func::zip(vec_string(50),vec_float(19));
    EXPECT_TRUE((std::is_same< std::pair<std::string, float>, decltype(z)::value_type>() ));
}

TEST(Zip, iterator){
    std::vector<int>   a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};

    auto x = func::zip(a,b);

    auto it  = x.begin();
    auto end = x.end();

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

    auto x = func::zip(a,b);
    const auto& z = x.begin();
    const auto& end = x.end();
    EXPECT_EQ(*(std::get<0>(z.source)), *a.begin());
    EXPECT_EQ(std::get<0>(z.source), a.begin());
    EXPECT_EQ(*(std::get<1>(z.source)), *b.begin());
    EXPECT_EQ(std::get<1>(z.source), b.begin());

    EXPECT_EQ(std::get<0>(end.source), a.end());
    EXPECT_EQ(std::get<1>(end.source), b.end());

    std::vector<std::pair<int,float>> lp;
    std::copy(x.begin(), x.end(), std::back_inserter(lp));

    EXPECT_EQ(lp[0].first, 1);
    EXPECT_EQ(lp[1].first, 2);
    EXPECT_EQ(lp[2].first, 3);
    EXPECT_EQ(lp[3].first, 4);

    EXPECT_FLOAT_EQ(lp[0].second, 0.1);
    EXPECT_FLOAT_EQ(lp[1].second, 0.2);
    EXPECT_FLOAT_EQ(lp[2].second, 0.3);
    EXPECT_FLOAT_EQ(lp[3].second, 0.4);
}

TEST(Zip, diff_size){
    std::vector<int> a = {1,4};
    std::vector<float> b = {.1,.2,.3,.4};

    auto x = func::zip(a,b);
    const auto& z = x.begin();
    const auto& end = x.end();

    std::vector<std::pair<int,float>> lp;
    std::copy(x.begin(), x.end(), std::back_inserter(lp));

    EXPECT_EQ(lp.size(), 2);
}

TEST(Zip, empty){
    // test the traits

    // test the struct
    std::vector<int> a;
    std::vector<float> b = {.1,.2,.3,.4};

    auto x = func::zip(a,b);
    EXPECT_TRUE(x.end().finish);
    EXPECT_TRUE(x.begin().finish);
    EXPECT_EQ(x.begin(), x.end());

    std::vector<std::pair<int,float>> lp;
    std::copy(x.begin(), x.end(), std::back_inserter(lp));

    EXPECT_EQ(lp.size(), 0);
}

TEST(Zip, three){
    std::vector<int> a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};
    std::list<double> c = {.1,.2,.3,.4};

    auto x = func::zip(a,b,c);

    EXPECT_TRUE((std::is_same<std::tuple<int,float,double>, decltype(x)::value_type>()));

    std::vector<std::tuple<int,float,double>> lp;
    std::copy(x.begin(), x.end(), std::back_inserter(lp));


    EXPECT_EQ( (std::get<0>(lp[0])), 1);
    EXPECT_EQ( (std::get<0>(lp[1])), 2);
    EXPECT_EQ( (std::get<0>(lp[2])), 3);
    EXPECT_EQ( (std::get<0>(lp[3])), 4);

    EXPECT_FLOAT_EQ( (std::get<1>(lp[0])), 0.1);
    EXPECT_FLOAT_EQ( (std::get<1>(lp[1])), 0.2);
    EXPECT_FLOAT_EQ( (std::get<1>(lp[2])), 0.3);
    EXPECT_FLOAT_EQ( (std::get<1>(lp[3])), 0.4);

    EXPECT_FLOAT_EQ( (std::get<2>(lp[0])), 0.1);
    EXPECT_FLOAT_EQ( (std::get<2>(lp[1])), 0.2);
    EXPECT_FLOAT_EQ( (std::get<2>(lp[2])), 0.3);
    EXPECT_FLOAT_EQ( (std::get<2>(lp[3])), 0.4);
}

TEST(Zip, pass_by_x){
    std::vector<int> a = {1,2,3,4};
    std::vector<int>& b = a;

    auto x = func::zip(a,b,std::list<double>({.1,.2,.3,.4})); //call zip with val, ref, rvalref

    EXPECT_TRUE((std::is_same<std::tuple<int,int,double>, decltype(x)::value_type>()));

    std::vector<std::tuple<int,int,double>> lp;
    std::copy(x.begin(), x.end(), std::back_inserter(lp));

    EXPECT_EQ( (std::get<0>(lp[0])), 1);
    EXPECT_EQ( (std::get<0>(lp[1])), 2);
    EXPECT_EQ( (std::get<0>(lp[2])), 3);
    EXPECT_EQ( (std::get<0>(lp[3])), 4);

    EXPECT_EQ( (std::get<1>(lp[0])), 1);
    EXPECT_EQ( (std::get<1>(lp[1])), 2);
    EXPECT_EQ( (std::get<1>(lp[2])), 3);
    EXPECT_EQ( (std::get<1>(lp[3])), 4);

    EXPECT_FLOAT_EQ( (std::get<2>(lp[0])), 0.1);
    EXPECT_FLOAT_EQ( (std::get<2>(lp[1])), 0.2);
    EXPECT_FLOAT_EQ( (std::get<2>(lp[2])), 0.3);
    EXPECT_FLOAT_EQ( (std::get<2>(lp[3])), 0.4);

    auto it  = x.begin();
    EXPECT_EQ       (*(std::get<0>(it.source)), 1);
    EXPECT_FLOAT_EQ (*(std::get<2>(it.source)), 0.1);
    ++it;
    EXPECT_EQ       (*(std::get<0>(it.source)), 2);
    EXPECT_FLOAT_EQ (*(std::get<2>(it.source)), 0.2);
    ++it;
    EXPECT_EQ       (*(std::get<0>(it.source)), 3);
    EXPECT_FLOAT_EQ (*(std::get<2>(it.source)), 0.3);
    ++it;
    EXPECT_EQ       (*(std::get<0>(it.source)), 4);
    EXPECT_FLOAT_EQ (*(std::get<2>(it.source)), 0.4);
}

TEST(Zip, four){
    std::vector<int> a = {1,2,3,4};
    std::vector<float> b = {.1,.2,.3,.4};
    std::vector<double> c = {.1,.2,.3,.4};

    auto x = func::zip(a,b,c);
    EXPECT_TRUE((*x.begin() == std::tuple<int,float,double>(1,0.1,0.1)));
    EXPECT_TRUE(((x.begin()[3]) == std::tuple<int,float,double>(4,0.4,0.4)));
    EXPECT_TRUE((*(x.begin()+1) == std::tuple<int,float,double>(2,0.2,0.2)));
    EXPECT_TRUE((x.begin()+4) == x.end());
    EXPECT_EQ(*((x.begin()+3)-2), (std::tuple<int,float,double>(2,0.2,0.2)));
    EXPECT_EQ((x.end()-x.begin()),4);
}
