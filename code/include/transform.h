//
//  transform.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#pragma once
#ifndef functional_transform_h
#define functional_transform_h

#include "utils.h"

namespace func{
    
    template<typename Value, typename Source, typename Func>
    struct m_iterator{
        
        Func& f;
        Source s;
        
        using self_type = m_iterator<Value, Source, Func>;
        
        //TODO: standard says default constructed
        m_iterator(Func& f, const Source s)
        :f(f), s(s) {}
        
        template <typename A, typename B, typename F>
        m_iterator(const m_iterator<A,B,F>& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        m_iterator(m_iterator<A,B,F>&& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        bool operator == (const m_iterator<A,B,F>& o){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return this->s == o.s;
        }
        
        template <typename A, typename B, typename F>
        bool operator != (const m_iterator<A,B,F>& o){
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
            ++s;
            return *this;
        }
        self_type operator++(int){
            self_type cpy= *this;
            ++s;
            return cpy;
        }
    };
    
    
    template<typename N, typename R, typename C>
    struct transform_t{

        static_assert(validate_container_function<N, C>::value,
                      "the function does not accept the collection type as paramenter");
        
//        statuc_assert(typename std::is_reference<C>::value,
//                      "this must be a reference type");
        
        using value_type = R;
        
        C storage;
        std::function<R (N)> func;
        
        using iterator = m_iterator<R, typename std::remove_reference<C>::type::iterator, std::function<R (N)>>;
        using const_iterator = const m_iterator<R, typename std::remove_reference<C>::type::iterator, std::function<R (N)>>;
        
        transform_t(std::function<R (N)> f, C c)
        :storage(c), func(f){}
        
//        transform_t(std::function<R (N)> f, C&& c)
//        :storage(std::move(c)), func(f){}
        
        iterator begin(){
            return iterator(func, storage.begin());
        }
        
        iterator end(){
            return iterator(func, storage.end());
        }
        
        const_iterator begin() const{
            return const_iterator(func, storage.begin());
        }
        
        const_iterator end() const{
            return const_iterator(func, storage.end());
        }
        
        operator std::vector<R>(){
            return std::vector<R>(begin(), end());
        }
    };
    
    
    template <typename N, typename R, typename C>
    transform_t<N,R,C> transform(std::function<R (N)> f, C c){
        return transform_t<N,R,C>(f, c);
    }

    template <typename F, typename C>
    transform_t<typename get_lambda<F,C>::param_type, typename get_lambda<F,C>::return_type, C>
    transform(F f, C c){
        return transform_t<typename get_lambda<F,C>::param_type, typename get_lambda<F,C>::return_type, C>(f,c);
    }
}

#endif
