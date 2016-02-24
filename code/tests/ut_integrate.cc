#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <functional>

#include <vector>
#include <deque>
#include <array>

#include <map>
#include <list>

#include <random>


#include "transform.h"
#include "filter.h"
#include "zip.h"
#include "reduce.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEST(Filter, chain){
        // four steps chained.
        // - variable lambda
        // - inline lamba
        // - function type
        // - functor
        
        std::list<int> l = { 0,1,2,3,4,5};

        auto filter = [](int i) { return i>2; };
        auto func = [](int i) { return i+1; };
        
        auto x = func::transform(func, func::filter( filter, l));
        std::vector<int> res (x.begin(), x.end());
        EXPECT_THAT(res, testing::ElementsAre( 4, 5, 6 ));

        auto y = func::filter(filter, func::transform( func, l));
        std::vector<int> res2 (y.begin(), y.end());
        EXPECT_THAT(res2, testing::ElementsAre( 3, 4, 5, 6));
    }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define SIZE 10

int values[] = { -5, -4, -3, -2, -1,
                  1,  2,  3,  4,  5};

#define VALIDATE(output) \
    EXPECT_THAT(output, testing::ElementsAre(5.1, 3.1, 1.1, 1.1, 3.1, 5.1));

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
struct RandomAccessTest : public testing::Test {

    T input;

    void mysetup();
    virtual void SetUp(){
        mysetup();

        ASSERT_EQ(input.size(), SIZE);
        for (int i = 0; i < SIZE; ++i){
            input[i] = values[i];
        }
    }
};


template<>
void RandomAccessTest<std::vector<int>>::mysetup(){
    input.resize(SIZE);
}

template<>
void RandomAccessTest<std::deque<int>>::mysetup(){
    input.resize(SIZE);
}
template<typename T>
void RandomAccessTest<T>::mysetup(){
}


// TestTypes only contains some of the types as before
// in this example, floating point types are tested only with RandomAccessTest.
typedef ::testing::Types<std::vector<int>, std::array<int,SIZE> , std::deque<int>> RandAccessTypes;

TYPED_TEST_CASE(RandomAccessTest, RandAccessTypes);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TYPED_TEST(RandomAccessTest, NestedLoops){

    auto a = [](int x) -> unsigned { return x<0? -x: x; };
    auto filter = [](unsigned x) -> bool { return x%2; };
    auto b = [](unsigned x) -> float { return x+0.1; };

    std::list<float> output;

    for (int i =0; i < SIZE;  ++i){

        float x = a(this->input[i]);
        while (!filter(x) && i < SIZE){
            ++i;
            x = a(this->input[i]);
        };

        output.push_back( b(x) );
    }
    EXPECT_EQ(output.size(), 6);
    VALIDATE(output);
}

TYPED_TEST(RandomAccessTest, std_cpy_func){

    auto a = [](int x) -> unsigned { return x<0? -x: x; };
    auto filter = [](unsigned x) -> bool { return x%2; };
    auto b = [](unsigned x) -> float { return x+0.1; };

    std::list<float> output;

    auto x = func::transform(a, this->input);
    auto y = func::filter( filter, x);
    auto z = func::transform(b, y);

    std::copy(z.begin(), z.end(), std::back_inserter(output));
    EXPECT_EQ(output.size(), 6);
    VALIDATE(output);
}

TYPED_TEST(RandomAccessTest, std_cpy_func_nest){

    auto a = [](int x) -> unsigned { return x<0? -x: x; };
    auto filter = [](unsigned x) -> bool { return x%2; };
    auto b = [](unsigned x) -> float { return x+0.1; };

    std::list<float> output;

    auto x = func::transform(b, 
             func::filter( filter, 
             func::transform(a, this->input)));

    std::copy(x.begin(), x.end(), std::back_inserter(output));
    EXPECT_EQ(output.size(), 6);
    VALIDATE(output);
}

TYPED_TEST(RandomAccessTest, std_cpy_func_inline){

    std::list<float> output;

    auto x = func::transform([](unsigned x) -> float { return x+0.1; }, 
             func::filter([](unsigned x) -> bool { return x%2; }, 
             func::transform([](int x) -> unsigned { return x<0? -x: x; }, this->input)));

    std::copy(x.begin(), x.end(), std::back_inserter(output));
    EXPECT_EQ(output.size(), 6);
    VALIDATE(output);
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
struct WithIterators : public testing::Test {

    T input;

    void mysetup();
    virtual void SetUp(){
        
        mysetup(); 
        EXPECT_EQ(input.size(), SIZE);
    }
};

template<>
void WithIterators<std::list<int>>::mysetup(){
    std::copy(values, values+SIZE, std::back_inserter(input));
}
template<>
void WithIterators<std::vector<int>>::mysetup(){
    std::copy(values, values+SIZE, std::back_inserter(input));
}
template<>
void WithIterators<std::array<int,SIZE>>::mysetup(){
        for (int i=0; i< SIZE; ++i) input[i] = values[i];
}
template<>
void WithIterators<std::deque<int>>::mysetup(){
    std::copy(values, values+SIZE, std::back_inserter(input));
}
template<typename T>
void WithIterators<T>::mysetup(){
        FAIL() << "not implemented";
}

typedef ::testing::Types<std::list<int>, std::vector<int>, std::array<int,SIZE>, std::deque<int>> NonRandAccessTypes;

TYPED_TEST_CASE(WithIterators, NonRandAccessTypes);

TYPED_TEST(WithIterators, NestedLoops){

    auto a = [](int x) -> unsigned { return x<0? -x: x; };
    auto filter = [](unsigned x) -> bool { return x%2; };
    auto b = [](unsigned x) -> float { return x+0.1; };

    std::list<float> output;
    using it_t = typename TypeParam::iterator;

    for (it_t it = this->input.begin(); it != this->input.end();  ++it){

        float x = a(*it);
        while (!filter(x) && it != this->input.end()){
            ++it;
            x = a(*it);
        };

        output.push_back( b(x) );
    }
    EXPECT_EQ(output.size(), 6);
    VALIDATE(output);
}

TYPED_TEST(WithIterators, std_cpy_func){

    auto a = [](int x) -> unsigned { return x<0? -x: x; };
    auto filter = [](unsigned x) -> bool { return x%2; };
    auto b = [](unsigned x) -> float { return x+0.1; };

    std::list<float> output;

    auto x = func::transform(b,
             func::filter( filter,
             func::transform(a, this->input)));

    std::copy(x.begin(), x.end(), std::back_inserter(output));
    VALIDATE(output);
}

TYPED_TEST(WithIterators, std_cpy_func_inline){


    std::list<float> output;

    auto x = func::transform( [](unsigned x) -> float { return x+0.1; },
             func::filter( [](unsigned x) -> bool { return x%2; },
             func::transform( [](int x) -> unsigned { return x<0? -x: x; }, this->input)));

    std::copy(x.begin(), x.end(), std::back_inserter(output));
    VALIDATE(output);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEST(all, together_small){

    std::vector<int> input1 {1,2,3,4,5,6};
    std::list<unsigned> input2(input1.begin(), input1.end());

    auto x = func::zip( input2, 
            func::transform( [](unsigned x) -> float { return x+0.1; },
             func::filter( [](unsigned x) -> bool { return x%2; },
             func::transform( [](int x) -> unsigned { return x<0? -x: x; }, input1))));


    auto it = x.begin();
    for(int i =0; i < 3; ++i){
        EXPECT_EQ ((*it).first, input1[i]);
        std::cout << (*it).second << std::endl;
        ++it;
    }
    EXPECT_EQ(it, x.end());
}

TEST(all, together){

    std::vector<int> input1 (100);
    std::list<unsigned> input2(input1.begin(), input1.end());

    auto x = func::zip( input2, 
            func::transform( [](unsigned x) -> float { return x+0.1; },
             func::filter( [](unsigned x) -> bool { return x%2; },
             func::transform( [](int x) -> unsigned { return x<0? -x: x; }, input1))));

    std::vector<std::pair<char, int>> output;
    std::copy(x.begin(), x.end(), std::back_inserter(output));

    EXPECT_EQ(output.size(), 0);
}
