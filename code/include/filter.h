//
//  filter.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//
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
                ++s;
            }
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
            do{
                ++s;
            }while(s != end && !f(*s));
            return *this;
        }
        self_type operator++(int){
            self_type cpy= *this;
            do{
                ++s;
            }while(s != end && !f(*s));
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



