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
