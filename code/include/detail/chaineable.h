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

namespace func{
namespace detail{

    struct Reference_storage{ };

    struct Value_storage{ };

    template<typename C, typename Storage>
    struct chaineable_store_t;

    template<typename C>
    struct chaineable_store_t<C,Reference_storage>{

        using stored_type = C;
        C& storage;

        chaineable_store_t(C& c)
        :storage(c) {}

        chaineable_store_t(chaineable_store_t&& t)
        :storage(t.storage) {}

        chaineable_store_t(const chaineable_store_t&) = delete;

        C& operator* (){ return storage; }
        C* operator-> (){ return &storage; }
    };

    template<typename C>
    struct chaineable_store_t<C,Value_storage>{

        using stored_type = C;
        C storage;

        chaineable_store_t(C&& c)
        :storage(std::move(c)) {}

        chaineable_store_t(chaineable_store_t&& t)
        :storage(std::move(t.storage)) {}

        chaineable_store_t(const chaineable_store_t&) = delete;

        C& operator* (){ return storage; }
        C* operator-> (){ return &storage; }
    };

    template<
             typename FuncType,         // type of the function
             typename Container,        // container type
             typename Storage_type,     // kind of storage (reference or value)
             typename Value_type,       // what values this chainable will produce
             typename Iterator          // what kind of iterator this chaineable will provide
                 >
    struct chaineable_t{

        // FIXME: this is valid for transform but not for compact
      //  using elem_type =  typename detail::get_lambda<FuncType,C>::param_type;
      //  static_assert(validate_container_function<elem_type, C>::value,
      //                "the function does not accept the collection type as paramenter");

        using value_type = Value_type;
        using storage_t = detail::chaineable_store_t<Container, Storage_type>;

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

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    template <typename T>
    using ref_t =  chaineable_store_t<T, Reference_storage>;
    template <typename T>
    using val_t =  chaineable_store_t<T, Value_storage>;


    // helper template metafunction that selects the right storage according to the given input
    template <typename T>
    struct choose_storage;

    template <typename T>
    struct choose_storage<T&&> {
        using type = val_t<T>;
    };

    template <typename T>
    struct choose_storage<T&> {
        using type = ref_t<T>;
    };

    template <typename T>
    using choose_storage_t = typename choose_storage<T>::type;

}// detail namespace
}// func namespace
