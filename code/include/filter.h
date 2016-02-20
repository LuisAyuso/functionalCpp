//
//  filter.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//
#pragma once
#include <iterator>
#include <functional>

#include "utils.h"
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
        bool operator == (const FilterIterator<A,B,F>& o){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return this->s == o.s;
        }
        
        template <typename A, typename B, typename F>
        bool operator != (const FilterIterator<A,B,F>& o){
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
    
    
    template<typename N, typename C, typename Storage_type>
        using filter_t = detail::chaineable_t<
                       N, bool, C, Storage_type, 
                        FilterIterator<N, typename C::iterator, std::function<bool(N)>>
                            >;

    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    filter_t<N,C,detail::Reference_storage> filter(std::function<R (N)> f, C& c){
        return filter_t<N,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    filter_t<N,C,detail::Value_storage> filter(std::function<R (N)> f, C&& c){
        return filter_t<N,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename C>
    filter_t<typename get_lambda<F,C>::param_type, C, detail::Reference_storage>
    filter(F f, C& c){
        using N = typename get_lambda<F,C>::param_type;
        using R = typename get_lambda<F,C>::return_type;
        return filter_t<N,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename C>
    filter_t<typename get_lambda<F,C>::param_type, C, detail::Value_storage>
    filter(F f, C&& c){
        using N = typename get_lambda<F,C>::param_type;
        using R = typename get_lambda<F,C>::return_type;
        return filter_t<N,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }
}



