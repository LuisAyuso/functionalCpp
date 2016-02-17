//
//  reduce.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#ifndef functional_reduce_h
#define functional_reduce_h

#include <functional>
#include "utils.h"

namespace func{
    
    template <typename N, typename R, typename C>
    N reduce_aux(std::function<R (R, N)>& f, C& c, R def){
        auto it = c.begin();
        if (it != c.end()){
            R value = def;
            while (it != c.end()){
                value = f(value, *it);
                ++it;
            }
            return value;
        }
        return def;
    }

    
    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    R reduce(std::function<R (R,N)> f, C& c, R def = R()){
        return reduce_aux(f,c,def);
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    R reduce(std::function<R (R,N)> f, C&& c, R def = R()){
        return reduce_aux(f, c, def);
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename R, typename C>
    R reduce(F f, C& c, R def){
        using N = typename C::value_type;
        std::function<R(R,N)> func = f;
        return reduce_aux(func, c, def);
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename R, typename C>
    R reduce(F f, C&& c, R def){
        using N = typename C::value_type;
        std::function<R(R,N)> func = f;
        return reduce_aux(func, c, def);
    }

} // func namespace

#endif
