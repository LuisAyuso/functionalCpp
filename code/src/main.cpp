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

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <list>


#include "transform.h"
#include "filter.h"
#include "reduce.h"



int main(int argc, const char * argv[]) {
    
    
    std::list<int> l = { 0,1,2,3,4,5};
    
    auto filter = [](int i) { return i>2; };
    auto func = [](int i) { return i+1; };
    
    auto x = func::transform(func, func::filter( filter, l));
    std::vector<int> res (x.begin(), x.end());
    
    for (auto v : x) std::cout << v << ",";
    std::cout << std::endl;

//    {
//        // lambda in variable.
//        
//        std::vector<int> v;
//        for (int i =0;i<20;i++) v.push_back(i);
//        std::function<int (int)> f = [](int a) -> int { return a+1; };
//        
//        auto x = func::transform(f, v);
//        
//        for(auto it : x){
//            std::cout<< it << std::endl;
//        }
//    }
//    std::cout << "=============" << std::endl;
//    {
//        // inline lambda
//        
//        std::vector<int> v;
//        for (int i =0;i<15;i++) v.push_back(i);
//        
//        auto x = func::transform([](int a) -> int { return a-1; }, v);
//        
//        std::cout << "  ======  " << std::endl;
//        
//        for(auto it : x){
//            std::cout<< it << std::endl;
//        }
//    }
//   
//    std::cout << "=============" << std::endl;
//    {
//        // X value
//        
//        std::function<int (int)> f =[](int x) -> int {return x * 2;};
//        auto y = func::transform(f, std::vector<int>({1,2,3,4,5}));
//        
//        for(auto it = y.begin(); it != y.end(); ++it){
//            std::cout<< *it << std::endl;
//        }
//    }
//    
//    std::cout << "=============" << std::endl;
//    {
//        // function type
//        
//        std::map<int, float> m;
//        std::function<int (std::pair<const int,float>)> f =[](std::pair<int, float> x) -> int {return x.first;};
//        auto y = func::transform(f,m);
//        
//        for(auto it = y.begin(); it != y.end(); ++it){
//            std::cout<< *it << std::endl;
//        }
//    }
//    
////    std::cout << "=============" << std::endl;
////    {
////        // four steps chained.
////        // - variable lambda
////        // - inline lamba
////        // - function type
////        // - functor
////        
////        std::list<int> l = { 0,1,2,3,4,5};
////        
////        auto lambda1 = [](int a) -> int { return a+1; };
////        std::function<int (int)> lambda2 = [](int a) -> int { return a+1; };
////        struct Lambda3{
////            int operator()(int a){
////                return a+1;
////            }
////        } lambda3;
////        
////        auto x = func::transform(lambda1,
////                                 func::transform([](int a){return a+1;},
////                                                 func::transform(lambda2,
////                                                                 func::transform(lambda3, l))));
////      
////        for(int v : x)
////        std::cout << v << std::endl;
////        
//////        std::vector<int> res(x.begin(), x.end());
////    //    std::vector<int> res(l.begin(), l.end());
////        
////    }
//    
//    
//    std::cout << "=============" << std::endl;
//    {
//        // map type
//        
//        std::map<int, float> m;
//        for (int i =0;i<10;i++) m[i] = i*2.0;
//        
//        std::function<int (float)> f1 = [](float a) ->int { return a+1; };
//        std::function<float(std::pair<int,float>)> f2 =[](std::pair<int, float> x) -> float {
//            return x.second;
//        };
//        
//        auto y = func::transform(f1, func::transform(f2,m));
//        for(auto it = y.begin(); it != y.end(); ++it){
//            std::cout<< *it << std::endl;
//        }
//    }
//    
//    std::cout << "=============" << std::endl;
//    {
//        // FILTER:
//        
//        std::vector<int> v;
//        for (int i =0;i<10;i++) v.push_back(i);
//        std::function<bool (int)> f = [](int a) ->bool { return a>5; };
//
//        auto x = func::filter(f, v);
//        for (auto it : x)
//            std::cout<< it << std::endl;
//    }
//    std::cout << "=============" << std::endl;
//    
////    {
////        std::vector<int> v;
////        for (int i =0;i<10;i++) v.push_back(i);
////        
////        std::function<bool (float)> f1 = [](int a) ->bool { return a>10; };
////        std::function<int (int)> f2 = [](int a) { return a*2; };
////        
////        auto x = func::filter(f1, func::transform(f2, v));
////        for (auto it : x)
////            std::cout<< it << std::endl;
////    }
////    std::cout << "=============" << std::endl;
////        // reduce:
////    {
////        std::vector<int> v;
////        for (int i =0;i<10;i++) v.push_back(i);
////        
////        std::function<bool (float)> f1 = [](int a) ->bool { return a>10; };
////        std::function<int (int)> f2 = [](int a) { return a*2; };
////        std::function<int (int,int)> f3 = [](int a, int b) { return a*b; };
////        
////        auto x = func::reduce(f3, func::filter(f1, func::transform(f2, v)));
////        
////        std::cout<< x << std::endl;
////    }
//    

}

