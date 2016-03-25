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
#pragma once
#include <iterator>

#include "detail/utils.h"
#include "detail/iterators.h"
#include "detail/chaineable.h"

namespace func{
namespace it{

    template<typename Value>
    struct SequenceIterator: public detail::iterator_type<SequenceIterator<Value>, Value> {

        Value v;
        const Value step;
        size_t count;

        using self_type = SequenceIterator<Value>;

        SequenceIterator(const Value& v, const Value& step, const size_t count)
        : v(v), step(step), count(count) { }

        SequenceIterator()
        : count(0), step() {}

        SequenceIterator(const SequenceIterator& o)
        : v(o.v), step(o.step), count(o.count) { }

        SequenceIterator(SequenceIterator&& o)
        : v(std::move(o.v)), step(std::move(o.step)), count(std::move(o.count)) { }

        SequenceIterator& operator= (const SequenceIterator& o){
            v = o.v;
            count = o.count;
            assert(step == o.step && "step differs");
            return *this;
        }
        SequenceIterator& operator= (SequenceIterator&& o){
            v = o.v;
            count = o.count;
            assert(step == o.step && "step differs");
            return *this;
        }

        bool operator == (const SequenceIterator& o) const{
            return count == o.count;
        }

        bool operator != (const SequenceIterator& o) const{
            return !(*this == o);
        }

        Value operator* (){
            return v;
        }
        Value* operator->(){
            return &v;
        }

        self_type& operator++(){
            assert(count>0 && "sequence generator limit reached.");
            --count;
            v += step;
            return *this;
        }
        self_type operator++(int){
            assert(count>0 && "sequence generator limit reached.");
            --count;
            self_type cpy = *this;
            v += step;
            return cpy;
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        Value operator[](unsigned i) {
            assert(count-i > 0 && "sequence generator limit reached.");
            return v+i*step;
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        S operator+ (int i) {
            self_type cpy = *this;
            assert(count-i > 0 && "sequence generator limit reached.");
            cpy.v+=i*step;
            cpy.count-=i;
            return cpy;
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        S operator- (int i) {
            self_type cpy = *this;
            assert(count+i > 0 && "sequence generator limit reached.");
            cpy.v-=i*step;
            cpy.count+=i;
            return cpy;
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        typename std::iterator_traits<S>::difference_type operator- (const S& o) {
            return count-o.count;
        }
    };
} // it namespace

    template <typename T>
    struct sequenece_t {

        using value_type = T;
        using iterator = it::SequenceIterator<T>;

        const T init, step;
        const size_t count;

        /*
         * it only accepts rvalues to intialize,
         * a helper function needs to be used to construct the containers and pass them
        */
        sequenece_t(const T& init, const T& step, const size_t count = std::numeric_limits<size_t>::max())
        : init(init), step(step), count(count)
        {}

        iterator begin() {
            return iterator(init, step, count);
        }

        iterator end() {
            return iterator();
        }
    };


    template<typename T>
    sequenece_t<T> sequence(const T& init, const T& step, const size_t count = std::numeric_limits<size_t>::max()){
        return sequenece_t<T>(init, step, count);
    }

    sequenece_t<int> sequence(const int& init = 0, int step = 1){
        return sequenece_t<int>(init, step);
    }

    sequenece_t<unsigned> sequence(const unsigned& init = 0, unsigned step = 1){
        return sequenece_t<unsigned>(init, step);
    }

}
