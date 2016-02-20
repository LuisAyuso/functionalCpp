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

struct Copy_count {

    using value_type = int;
    using iterator = std::vector<int>::iterator;
    using const_iterator = std::vector<int>::const_iterator;

    std::vector<int> vec;
    unsigned& cpy;
    unsigned& mv;

    Copy_count (unsigned& cpy, unsigned& mv) : vec(100), cpy(cpy), mv(mv){}
    Copy_count (const Copy_count& o): vec(o.vec), cpy(o.cpy), mv(o.mv) { cpy++; }
    Copy_count ( Copy_count&& o): vec(o.vec), cpy(o.cpy), mv(o.mv) { mv++; }

    decltype(vec.begin()) begin() { return vec.begin(); }
    decltype(vec.end()) end() { return vec.end(); }
};

TEST(Transform, check_copys){

        unsigned cpy = 0;
        unsigned mv = 0;
        Copy_count v(cpy, mv);

        std::function<int (int)> a = [](int a) -> int { return a+1; };
        std::function<int (int)> b = [](int a) -> int { return a+1; };
        std::function<int (int)> c = [](int a) -> int { return a+1; };
        std::function<int (int)> d = [](int a) -> int { return a+1; };
        std::function<int (int)> e = [](int a) -> int { return a+1; };

        auto x = func::transform(a, 
                 func::transform(b, 
                 func::transform(c, 
                 func::transform(d, 
                 func::transform(e, v)))));

        std::vector<int> res (x.begin(), x.end());

        EXPECT_EQ(res.size(), 100);
        EXPECT_EQ(mv, 0);
        EXPECT_EQ(cpy, 0);
}


TEST(Transform, check_moves){

        unsigned cpy = 0;
        unsigned mv = 0;

        std::function<int (int)> a = [](int a) -> int { return a+1; };
        std::function<int (int)> b = [](int a) -> int { return a+1; };
        std::function<int (int)> c = [](int a) -> int { return a+1; };
        std::function<int (int)> d = [](int a) -> int { return a+1; };
        std::function<int (int)> e = [](int a) -> int { return a+1; };

        auto x = func::transform(a, 
                 func::transform(b, 
                 func::transform(c, 
                 func::transform(d, 
                 func::transform(e, Copy_count(cpy, mv))))));

        std::vector<int> res (x.begin(), x.end());

        EXPECT_EQ(res.size(), 100);
        EXPECT_EQ(mv, 10);  // 2 moves per chain (create storage, and move in chainable)
        EXPECT_EQ(cpy, 0);
}


struct Copy_count_functor{

    unsigned& cpy;
    unsigned& mv;

    Copy_count_functor(unsigned& cpy, unsigned& mv) : cpy(cpy), mv(mv){}
    Copy_count_functor(const Copy_count& o): cpy(o.cpy), mv(o.mv) { cpy++; }
    Copy_count_functor(Copy_count&& o): cpy(o.cpy), mv(o.mv)      { mv++; }

    Copy_count_functor& operator= (const Copy_count_functor& o) = delete;
    
    int operator() (int a){ return a; }
};

TEST(Transform, functor_copies){

        unsigned cpy = 0;
        unsigned mv = 0;

        Copy_count_functor a(cpy, mv);
        Copy_count_functor b(cpy, mv);
        std::vector<int> v(100);

        auto x = func::transform(a, 
                 func::transform(b, v));

        std::vector<int> res (x.begin(), x.end());

        EXPECT_EQ(res.size(), 100);
        EXPECT_EQ(mv, 0);
        EXPECT_EQ(cpy, 0);
}

TEST(Transform, functor_moves){

        unsigned cpy = 0;
        unsigned mv = 0;

        std::vector<int> v(100);

        auto x = func::transform(Copy_count_functor(cpy, mv), v);

        std::vector<int> res (x.begin(), x.end());

        EXPECT_EQ(res.size(), 100);
        EXPECT_EQ(mv, 0);
        EXPECT_EQ(cpy, 0);
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
   
//TEST(Transform, x_value){
//        // X value
//        std::function<int (int)> f =[](int x) -> int {return x * 2;};
//        auto x = func::transform(f, std::vector<int>({1,2,3,4,5}));
//        
//        std::vector<int> res (x.begin(), x.end());
//        EXPECT_THAT(res, ElementsAre(2,4,6,8,10));
//}
    
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
    const unsigned BenchmarkSize = 1024 * 1024;
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


TEST_F(BenchmarkVectorTest, Base){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  a(b(c(d(input[i]))));
    }
}

TEST_F(BenchmarkVectorTest, post_func){

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
TEST_F(BenchmarkVectorTest, pre_func){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    auto it = x.begin();
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] = *it;
        ++it ;
    }
}

TEST_F(BenchmarkVectorTest, Bad){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  d(input[i]);
    }
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  c(res[i]);
    }
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  b(res[i]);
    }
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  a(res[i]);
    }
}

class BenchmarkListTest : public ::testing::Test {
protected:
    static const unsigned BenchmarkSize =  1024 * 1024;

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

TEST_F(BenchmarkListTest, post_func){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    for (auto it = x.begin(); it != x.end(); ++it){
        res.push_back(*it);
        it++;
    }
}
TEST_F(BenchmarkListTest, pre_func){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    for (auto it = x.begin(); it != x.end(); ++it){
        res.push_back(*it);
        ++it;
    }
}

TEST_F(BenchmarkListTest, bad){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    std::list<int> r1,r2,r3,r4;

    for (auto it = input.begin(); it != input.end(); ++it){
        r1.push_back(a(*it));
    }
    for (auto it = r1.begin(); it != r1.end(); ++it){
        r2.push_back(b(*it));
    }
    for (auto it = r2.begin(); it != r2.end(); ++it){
        r3.push_back(c(*it));
    }
    for (auto it = r3.begin(); it != r3.end(); ++it){
        res.push_back(d(*it));
    }
}




class BenchmarkArrayTest : public ::testing::Test {
protected:
    //static const unsigned BenchmarkSize = 4 * 1024 * 1024;
    static const unsigned BenchmarkSize = 1024 * 1024;

    std::array<float,BenchmarkSize> input;
    std::array<float,BenchmarkSize> res;

    virtual void SetUp() {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);

        for (unsigned i = 0; i < BenchmarkSize; ++i){
            input[i] = dist(rd);
        }
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

TEST_F(BenchmarkArrayTest, post_func){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    auto it = x.begin();
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] = *it;
        it++;
    }
}

TEST_F(BenchmarkArrayTest, pre_func){

    auto x = func::transform([](float a){ return a-1; },
             func::transform([](float a){ return a/4; },
             func::transform([](float a){ return a*3; },
             func::transform([](float a){ return a+1; }, input))));

    auto it = x.begin();
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] = *it;
        ++it;
    }
}

TEST_F(BenchmarkArrayTest, bad){

    auto a = [](int a){ return a-1; };
    auto b = [](int a){ return a/4; };
    auto c = [](int a){ return a*3; };
    auto d = [](int a){ return a+1; };

    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  d(input[i]);
    }
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  c(res[i]);
    }
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  b(res[i]);
    }
    for (unsigned i = 0; i < BenchmarkSize; ++i){
        res[i] =  a(res[i]);
    }
}

