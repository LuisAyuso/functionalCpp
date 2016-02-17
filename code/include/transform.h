//
//  transform.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#pragma once
#include <iterator>

#include "utils.h"
#include "detail/chaineable.h"

namespace func{
    
    template<typename Value, typename Source, typename Func>
    struct TransforIterator : public std::iterator<std::input_iterator_tag, Value>{

        Func f;
        Source s;
        
        using self_type = TransforIterator<Value, Source, Func>;

        TransforIterator(Func f, const Source s)
        :f(f), s(s) {}
        
        template <typename A, typename B, typename F>
        TransforIterator(const TransforIterator<A,B,F>& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        TransforIterator(TransforIterator<A,B,F>&& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        bool operator == (const TransforIterator<A,B,F>& o){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return this->s == o.s;
        }
        
        template <typename A, typename B, typename F>
        bool operator != (const TransforIterator<A,B,F>& o){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return s != o.s;
        }
        
        Value operator* (){
            return f(*s);
        }
        Value operator->(){
            return f(*s);
        }

        self_type& operator++(){
            s++;
            return *this;
        }
        self_type operator++(int){
            self_type cpy= *this;
            s++;
            return cpy;
        }
    };


   template <typename N, typename R, typename C, typename Storage_type> 
    using transform_t = detail::chaineable_t<
                            N, R,C,Storage_type, // forward paramenters
                            TransforIterator<R,typename std::remove_reference<C>::type::iterator,std::function<R(N)>> // specific iterator type for transformation
                                >;


    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    transform_t<N,R,C,detail::Reference_storage> transform(std::function<R (N)> f, C& c){
        return transform_t<N,R,C,detail::Reference_storage> (f, detail::transform_store_t<C,detail::Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    transform_t<N,R,C,detail::Value_storage> transform(std::function<R (N)> f, C&& c){
        return transform_t<N,R,C,detail::Value_storage> (f, detail::transform_store_t<C,detail::Value_storage> (std::move(c)));
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename C>
    transform_t<typename get_lambda<F,C>::param_type, typename get_lambda<F,C>::return_type, C, detail::Reference_storage>
    transform(F f, C& c){
        using N = typename get_lambda<F,C>::param_type;
        using R = typename get_lambda<F,C>::return_type;
        return transform_t<N,R,C,detail::Reference_storage> (f, detail::transform_store_t<C,detail::Reference_storage> (c));
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename C>
    transform_t<typename get_lambda<F,C>::param_type, typename get_lambda<F,C>::return_type, C, detail::Value_storage>
    transform(F f, C&& c){
        using N = typename get_lambda<F,C>::param_type;
        using R = typename get_lambda<F,C>::return_type;
        return transform_t<N,R,C,detail::Value_storage> (f, detail::transform_store_t<C,detail::Value_storage> (std::move(c)));
    }
}

