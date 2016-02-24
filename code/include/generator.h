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
#pragma once
#include <iterator>

#include "detail/utils.h"
#include "detail/chaineable.h"

namespace func{
    
    template<typename Value>
    struct SequenceIterator: public std::iterator<std::input_iterator_tag, Value>{
        
        Value v;
        const Value step;
        const bool end;
        
        using self_type = SequenceIterator<Value>;
        
        SequenceIterator(const Value& v, const Value& step)
        : v(v), step(step), end(false) { }

        SequenceIterator()
        : end(true) {}

        SequenceIterator(const SequenceIterator& o)
        : v(o.v), step(o.step), end(o.end) { }

        SequenceIterator(SequenceIterator&& o) 
        : v(std::move(o.v)), step(std::move(o.step)), end(o.end) { }

        SequenceIterator& operator= (const SequenceIterator& o){
            v = o.v;
            return *this;
        }
        SequenceIterator& operator= (SequenceIterator&& o){
            v = o.v;
            return *this;
        }

        bool operator == (const SequenceIterator& o) const{
            return end == o.end && v == o.v;
        }
        
        bool operator != (const SequenceIterator& o) const{
            return !(*this == o);
        }
        
        Value operator* (){
            return v;
        }
        Value& operator->(){
            return &v;
        }
        
        self_type& operator++(){
            v += step;
            return *this;
        }
        self_type operator++(int){
            self_type cpy = *this;
            v += step;
            return cpy;
        }
    };

    template <typename T>
    struct sequenece_t {

        using value_type = T;
        using iterator = SequenceIterator<T>;

        const T init, step;

        /* 
         * it only accepts rvalues to intialize, 
         * a helper function needs to be used to construct the containers and pass them
        */
        sequenece_t(const T& init, const T& step) 
        : init(init), step(step)
        {}

        iterator begin() {
            return SequenceIterator<T>(init, step);
        }

        iterator end() {
            return SequenceIterator<T>();
        }
    };
    

    template<typename T>
    sequenece_t<T> sequence(const T& init, const T& step){
        return sequenece_t<T>(init, step);
    }

    sequenece_t<int> sequence(const int& init = 0, int step = 1){
        return sequenece_t<int>(init, step);
    }

    sequenece_t<unsigned> sequence(const unsigned& init = 0, unsigned step = 1){
        return sequenece_t<unsigned>(init, step);
    }

}



