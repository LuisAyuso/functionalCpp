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
#include <cassert>

#include "detail/utils.h"
#include "detail/iterators.h"
#include "detail/chaineable.h"

namespace func{
namespace it{
    template<typename Value, typename Source, typename Func>
    struct FilterIterator: public detail::iterator_type<FilterIterator<Value, Source, Func>, Value>  {

        Func& f;
        Source s;
        Source end;
        bool finish;

        using self_type = FilterIterator<Value, Source, Func>;

        FilterIterator(Func& f, const Source& beg, const Source& end)
        :f(f), s(beg), end(end) {
            while(s != end && !f(*s)){
                ++s;
            }
            finish = end == s;
        }

        FilterIterator(const FilterIterator& o)
          : f(o.f), s(o.s), end(o.end), finish(o.finish){ }

        FilterIterator(FilterIterator&& o)
          : f(o.f), s(std::move(o.s)), end(std::move(o.end)), finish(o.finish) { }

        FilterIterator& operator= (const FilterIterator& o){
            s = o.s;
            end = o.end;
            finish = o.finish;
            return *this;
        }
        FilterIterator& operator= (FilterIterator&& o){
            std::swap(s, o.s);
            std::swap(end, o.end);
            finish = o.finish;
            return *this;
        }

        template <typename A, typename B, typename F>
        bool operator == (const FilterIterator<A,B,F>& o) const{
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            if (finish && finish == o.finish) return true;
            return s == o.s;
        }

        template <typename A, typename B, typename F>
        bool operator != (const FilterIterator<A,B,F>& o) const{
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return !(*this == o);
        }

        Value operator* (){
            assert(s != end && "deref and end iterator");
            return *s;
        }

        self_type& operator++(){
            assert(s != end && "move and end iterator");
            do{
                ++s;
            }while(s != end && !f(*s));
            finish = end == s;
            return *this;
        }
        self_type operator++(int){
            assert(s != end && "move and end iterator");
            self_type cpy= *this;
            do{
                ++s;
            }while(s != end && !f(*s));
            finish = end == s;
            return cpy;
        }
    };
} // it namespace

    template <typename FuncType, typename Container, typename Storage_type>
    using filter_t = detail::chaineable_t<
                            FuncType,
                            Container,
                            Storage_type,
                            typename Container::value_type,
                            it::FilterIterator<typename Container::value_type, typename Container::iterator, FuncType > // specific iterator type for transformation
                                >;

    // for function type
    // lvalue collection
    template <typename FuncType, typename C>
    filter_t<FuncType,C,detail::Reference_storage> filter(FuncType f, C& c){
        return filter_t<FuncType, C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename FuncType, typename C>
    filter_t<FuncType,C,detail::Value_storage> filter(FuncType f, C&& c){
        return filter_t<FuncType,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }

//    // for lambda type
//    // lvalue collection
//    template <typename FuncType, typename C>
//    filter_t<FuncType, C, detail::Reference_storage>
//    filter(FuncType f, C& c){
//        return filter_t<FuncType,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
//    }
//
//    // for lambda type
//    // xvalue collection
//    template <typename F, typename C>
//    filter_t<typename get_lambda<F,C>::param_type, C, detail::Value_storage>
//    filter(F f, C&& c){
//        using N = typename get_lambda<F,C>::param_type;
//        return filter_t<N,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
//    }
}
