/** 
	FunctionalCpp,  A header only libray for chaineable functional operations 
	in C++ collections
    Copyright (C) 2016 Luis F. Ayuso

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

#ifndef functional_reduce_h
#define functional_reduce_h

#include <functional>
#include "detail/utils.h"

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
