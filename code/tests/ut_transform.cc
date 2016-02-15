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

TEST(Transform, lambda1){
        // lambda in variable.
        std::vector<int> v;
        for (int i =0;i<10;i++) v.push_back(i);
        std::function<int (int)> f = [](int a) -> int { return a+1; };
        
        auto x = func::transform(f, v);

        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(1,2,3,4,5,6,7,8,9,10));
}

TEST(Transform_array, lambda1){
        // lambda in variable.
        std::array<int,10> v;
        for (int i =0;i<10;i++) v[i] = i;
        std::function<int (int)> f = [](int a) -> int { return a+1; };
        
        auto x = func::transform(f, v);

        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(1,2,3,4,5,6,7,8,9,10));
}



TEST(Transform, inline_lambda){
        // inline lambda
        std::vector<int> v;
        for (int i =0;i<10;i++) v.push_back(i);
        
        auto x = func::transform([](int a) -> int { return a-1; }, v);
        
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(-1,0,1,2,3,4,5,6,7,8));
}
   
TEST(Transform, x_value){
        // X value
        std::function<int (int)> f =[](int x) -> int {return x * 2;};
        auto x = func::transform(f, std::vector<int>({1,2,3,4,5}));
        
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(2,4,6,8,10));
}
    
TEST(Transform, std_function){
        // function type
        std::map<int, float> m;
        m[1] = 0.0;
        m[2] = 0.1;
        std::function<int (std::pair<const int,float>)> f =[](std::pair<int, float> x) -> int {return x.first;};
        auto x = func::transform(f,m);
        
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(1,2));
}
    
TEST(Transform, chain){
        // four steps chained.
        // - variable lambda
        // - inline lamba
        // - function type
        // - functor
        
        std::list<int> l = { 0,1,2,3,4,5};
        
        auto lambda1 = [](int a) -> int { return a+1; };
        std::function<int (int)> lambda2 = [](int a) -> int { return a+1; };
        struct Lambda3{
            int operator()(int a){
                return a+1;
            }
        } lambda3;
        
        auto x = func::transform( lambda1,
                 func::transform( [](int a){return a+1;},
                 func::transform( lambda2,
                 func::transform( lambda3, l))));
      
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, ElementsAre(4,5,6,7,8,9));
}


class BenchmarkVectorTest : public ::testing::Test {
protected:
    const unsigned BenchmarkSize = 4 * 1024 * 1024;
    //const unsigned BenchmarkSize = 10;

    std::vector<float> input;
    std::vector<float> res;

    virtual void SetUp() {

        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);

        input.resize(BenchmarkSize);

        for (unsigned i = 0; i < BenchmarkSize; ++i){
            input[i] = dist(rd);
        }
        res.resize(BenchmarkSize);
    }

    //virtual void TearDown() { }
};


TEST_F(BenchmarkVectorTest, Base_vector){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  a(b(c(d(input[i]))));
    }
}

TEST_F(BenchmarkVectorTest, func_vector){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    auto it = x.begin();
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] = *it;
        it ++;
    }
}

class BenchmarkListTest : public ::testing::Test {
protected:
    static const unsigned BenchmarkSize = 4 * 1024 * 1024;

    std::list<float> input;
    std::list<float> res;

    virtual void SetUp() {

        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);

        for (unsigned i = 0; i < BenchmarkSize; ++i){
            input.insert(input.begin(), dist(rd));
        }
    }

    //virtual void TearDown() { }
};


TEST_F(BenchmarkListTest, Base){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    for (auto it = input.begin(); it != input.end(); ++it){
        input.insert(input.begin(), a(b(c(d(*it)))));
    }
}

TEST_F(BenchmarkListTest, func){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    for (auto it = x.begin(); it != x.end(); ++it){
        input.insert(input.begin(), *it);
    }
}



class BenchmarkArrayTest : public ::testing::Test {
protected:
    //static const unsigned BenchmarkSize = 4 * 1024 * 1024;
    static const unsigned BenchmarkSize = 1024 * 1024;

    std::array<float,BenchmarkSize> input;
    std::array<float,BenchmarkSize> res;

    virtual void SetUp() {
    std::cout << "start! " << std::endl;

        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);

        for (unsigned i = 0; i < BenchmarkSize; ++i){
            input[i] = dist(rd);
        }
    std::cout << "stop! " << std::endl;
    }

    //virtual void TearDown() { }
};


TEST_F(BenchmarkArrayTest, Base){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  a(b(c(d(input[i]))));
    }
}

TEST_F(BenchmarkArrayTest, func){

std::cout << "yo! " << std::endl;

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    auto it = x.begin();
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        std::cout << "-" << i << " (" << *it << ")";
        res[i] = *it;
        ++it;
        std::cout << std::endl;
    }
}


