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

namespace func{
    
    template<typename Value, typename Source, typename Func>
    struct m_iterator : public std::iterator<std::input_iterator_tag, Value>{

        Func f;
        Source s;
        
        using self_type = m_iterator<Value, Source, Func>;

        //m_iterator():f(), s(){}
        
        m_iterator(Func f, const Source s)
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
            s++;
            return *this;
        }
        self_type operator++(int){
            self_type cpy= *this;
            s++;
            return cpy;
        }
    };

    struct Reference_storage{
    };

    struct Value_storage{
    };

    namespace detail{

        template<typename C, typename Storage>
        struct transform_store_t;

        template<typename C>
        struct transform_store_t<C,Reference_storage>{

            C& storage;

            transform_store_t(C& c)
            :storage(c) {}

            transform_store_t(transform_store_t&& t)
            :storage(t.storage) {}

            transform_store_t(const transform_store_t&) = delete;
        };

        template<typename C>
        struct transform_store_t<C,Value_storage>{

            C storage;

            transform_store_t(C&& c)
            :storage(std::move(c)) {}

            transform_store_t(transform_store_t&& t)
            :storage(std::move(t.storage)) {}

            transform_store_t(const transform_store_t&) = delete;
        };

    }// detail namespace


    
    template<typename N, typename R, typename C, typename Storage_type>
    struct transform_t{

        static_assert(validate_container_function<N, C>::value,
                      "the function does not accept the collection type as paramenter");
        
        using value_type = R;
        using function_type = std::function<R (N)>;
        using storage_t = detail::transform_store_t<C, Storage_type>;

        function_type func;
        storage_t store;

        transform_t(function_type func, storage_t&& store) : func(func), store(std::move(store)) {}
        
        using iterator = m_iterator<R, typename std::remove_reference<C>::type::iterator, std::function<R (N)>>;
        using const_iterator = const m_iterator<R, typename std::remove_reference<C>::type::iterator, std::function<R (N)>>;
        
        transform_t (const transform_t & ) = delete;
        transform_t (transform_t&& o) :func(o.func), store(std::move(o.store)) {}
        
        iterator begin(){
            return iterator(func, store.storage.begin());
        }
        
        iterator end(){
            return iterator(func, store.storage.end());
        }
        
        const_iterator begin() const{
            return const_iterator(func, store.storage.begin());
        }
        
        const_iterator end() const{
            return const_iterator(func, store.storage.end());
        }
    };

    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    transform_t<N,R,C,Reference_storage> transform(std::function<R (N)> f, C& c){
        return transform_t<N,R,C,Reference_storage> (f, detail::transform_store_t<C,Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    transform_t<N,R,C,Value_storage> transform(std::function<R (N)> f, C&& c){
        return transform_t<N,R,C,Value_storage> (f, detail::transform_store_t<C,Value_storage> (std::move(c)));
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename C>
    transform_t<typename get_lambda<F,C>::param_type, typename get_lambda<F,C>::return_type, C, Reference_storage>
    transform(F f, C& c){
        using N = typename get_lambda<F,C>::param_type;
        using R = typename get_lambda<F,C>::return_type;
        return transform_t<N,R,C,Reference_storage> (f, detail::transform_store_t<C,Reference_storage> (c));
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename C>
    transform_t<typename get_lambda<F,C>::param_type, typename get_lambda<F,C>::return_type, C, Value_storage>
    transform(F f, C&& c){
        using N = typename get_lambda<F,C>::param_type;
        using R = typename get_lambda<F,C>::return_type;
        return transform_t<N,R,C,Value_storage> (f, detail::transform_store_t<C,Value_storage> (std::move(c)));
    }
}

