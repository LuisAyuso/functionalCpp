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

TEST(Transform, lambda1){
        // lambda in variable.
        std::vector<int> a;
        std::vector<int> b;

       for (int i =0;i<10;i++) {
            a.push_back(i);
            b.push_back(i);
       }

       // auto x = func::zip(a, b);

       // std::vector<int> res (x.begin(), x.end());
       // EXPECT_THAT(res, ElementsAre(1,2,3,4,5,6,7,8,9,10));
}

