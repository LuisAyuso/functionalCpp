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
namespace detail{
    
    struct Reference_storage{ };

    struct Value_storage{ };

    template<typename C, typename Storage>
    struct chaineable_store_t;

    template<typename C>
    struct chaineable_store_t<C,Reference_storage>{

        C& storage;

        chaineable_store_t(C& c)
        :storage(c) {}

        chaineable_store_t(chaineable_store_t&& t)
        :storage(t.storage) {}

        chaineable_store_t(const chaineable_store_t&) = delete;
    };

    template<typename C>
    struct chaineable_store_t<C,Value_storage>{

        C storage;

        chaineable_store_t(C&& c)
        :storage(std::move(c)) {}

        chaineable_store_t(chaineable_store_t&& t)
        :storage(std::move(t.storage)) {}

        chaineable_store_t(const chaineable_store_t&) = delete;
    };
    
    template<
             typename FuncType,          // type of the function
             typename C,                // container type
             typename Storage_type,     // kind of storage (reference or value)
             typename Iterator          // what kind of iterator this chaineable will provide
                 > 
    struct chaineable_t{

        using elem_type =  typename get_lambda<FuncType,C>::param_type;
        static_assert(validate_container_function<elem_type, C>::value,
                      "the function does not accept the collection type as paramenter");
       
        using value_type =  typename get_lambda<FuncType,C>::return_type;
        using storage_t = detail::chaineable_store_t<C, Storage_type>;

        FuncType func;
        storage_t store;

        chaineable_t(FuncType func, storage_t&& store) : func(func), store(std::move(store)) {}
        
        using iterator = Iterator;
        using const_iterator = const Iterator;
        
        chaineable_t (const chaineable_t & ) = delete;
        chaineable_t (chaineable_t&& o) :func(o.func), store(std::move(o.store)) {}

        chaineable_t operator= (const chaineable_t&) = delete;
        chaineable_t operator= (chaineable_t&&) = delete;
        
        iterator begin(){
            return iterator(func, store.storage.begin(), store.storage.end());
        }
        
        iterator end(){
            return iterator(func, store.storage.end(), store.storage.end());
        }
        
        const_iterator begin() const{
            return const_iterator(func, store.storage.begin());
        }
        
        const_iterator end() const{
            return const_iterator(func, store.storage.end());
        }
    };

 // Helper metafunctions

    template<typename T, typename Default>
    struct get_storage_kind{
        typedef typename T::storage_t type;
    };



}// detail namespace
}// func namespace

