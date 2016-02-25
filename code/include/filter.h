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
#include "detail/chaineable.h"

namespace func{
    
    template<typename Value, typename Source, typename Func>
    struct FilterIterator: public std::iterator<std::input_iterator_tag, Value>{
        
        Func& f;
        Source s;
        Source end;
        
        using self_type = FilterIterator<Value, Source, Func>;
        
        FilterIterator(Func& f, const Source beg, const Source end)
        :f(f), s(beg), end(end) {
            while(s != end && !f(*s)){
                std::cout << "+";
                ++s;
            }
            std::cout << std::endl;
        }

        FilterIterator(const FilterIterator& o)
          : f(o.f), s(o.s), end(o.end) { }

        FilterIterator(FilterIterator&& o)
          : f(o.f), s(std::move(o.s)), end(std::move(o.end)) { }

        FilterIterator& operator= (const FilterIterator& o){
            s = o.s;
            return *this;
        }
        FilterIterator& operator= (FilterIterator&& o){
            std::swap(s, o.s);
            return *this;
        }
        
        template <typename A, typename B, typename F>
        FilterIterator(const FilterIterator<A,B,F>& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        FilterIterator(FilterIterator<A,B,F>&& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        bool operator == (const FilterIterator<A,B,F>& o) const{
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return this->s == o.s;
        }
        
        template <typename A, typename B, typename F>
        bool operator != (const FilterIterator<A,B,F>& o) const{
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return s != o.s;
        }
        
        Value operator* (){
            return *s;
        }
        Value operator->(){
            return *s;
        }
        
        self_type& operator++(){
            if(s == end) return *this;
            do{
                std::cout << "+";
                ++s;
            }while(s != end && !f(*s));
                std::cout <<  std::endl;
            return *this;
        }
        self_type operator++(int){
            if(s == end) return *this;
            self_type cpy= *this;
            do{
                std::cout << "+";
                ++s;
            }while(s != end && !f(*s));
                std::cout <<  std::endl;
            return cpy;
        }
    };
    
    

    template <typename FuncType, typename C, typename Storage_type>
    using filter_t = detail::chaineable_t<
                            FuncType, C, Storage_type, // forward paramenters
                            FilterIterator<typename C::value_type, typename C::iterator, FuncType > // specific iterator type for transformation
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



