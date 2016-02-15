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
        
        auto x = func::filter([](int a) -> bool{ return a%2; }, v);
        
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(1,3,5,7,9));
    }
//   
//TEST(Filter, x_value){
//        // X value
//        std::function<int (int)> f =[](int x) -> int {return x * 2;};
//        auto x = func::transform(f, std::vector<int>({1,2,3,4,5}));
//        
//        std::vector<int> res (x.begin(), x.end());
//        EXPECT_THAT(res, ElementsAre(2,4,6,8,10));
//    }
//    
//TEST(Filter, std_function){
//        // function type
//        std::map<int, float> m;
//        m[1] = 0.0;
//        m[2] = 0.1;
//        std::function<int (std::pair<const int,float>)> f =[](std::pair<int, float> x) -> int {return x.first;};
//        auto x = func::transform(f,m);
//        
//        std::vector<int> res (x.begin(), x.end());
//        EXPECT_THAT(res, ElementsAre(1,2));
//    }
//    
//TEST(Filter, chain){
//        // four steps chained.
//        // - variable lambda
//        // - inline lamba
//        // - function type
//        // - functor
//        
//        std::list<int> l = { 0,1,2,3,4,5};
//        
//        auto lambda1 = [](int a) -> int { return a+1; };
//        std::function<int (int)> lambda2 = [](int a) -> int { return a+1; };
//        struct Lambda3{
//            int operator()(int a){
//                return a+1;
//            }
//        } lambda3;
//        
//        auto x = func::transform(lambda1,
//                                 func::transform([](int a){return a+1;},
//                                                 func::transform(lambda2,
//                                                                 func::transform(lambda3, l))));
//      
//        std::vector<int> res (x.begin(), x.end());
//        EXPECT_THAT(res, ElementsAre(4,5,6,7,8,9));
//    }
