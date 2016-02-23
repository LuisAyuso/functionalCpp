//
//  transform.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#pragma once
#include <iterator>
#include <cassert>

#include "detail/utils.h"
#include "detail/chaineable.h"

namespace func{
    namespace {
        template <typename... Args>
        struct get_value_type {
            using type = std::tuple<Args...>;
        };

        template <typename T>
        struct get_value_type<T> {
            using type = T;
        };

        template <typename T, typename V>
        struct get_value_type<T,V> {
            using type = std::pair<T,V>;
        };

        template <typename... Args>
        using get_value_type_t = typename get_value_type<Args...>::type;

        /*************GET TUPLE ELEMENT***************/

        template <typename Iter, typename T, unsigned N>
        struct aux {
            template <typename... Args>
            static Iter get_begin(T& tuple, Args... storages) {
                return aux<Iter,T,N-1>::get_begin(tuple, std::get<N-1>(tuple).begin(), storages...);
            }
            template <typename... Args>
            static Iter get_end(T& tuple, Args... storages) {
                return aux<Iter,T,N-1>::get_end(tuple, std::get<N-1>(tuple).end(), storages...);
            }
        };

        template <typename Iter, typename T>
        struct aux<Iter,T,0> {
            template <typename... Args>
            static Iter get_begin(T& tuple, Args... storages) {
                return Iter(typename Iter::inner_type(storages...));
            }
            template <typename... Args>
            static Iter get_end(T& tuple, Args... storages) {
                return Iter(typename Iter::inner_type(storages...));
            }
        };

        /*************TRANSFORM TUPLE***************/
        template <typename T1, typename T2, unsigned N>
        struct trf {
            static void apply_pp(T1& input, T2& output) {
                std::get<N>(output) = ++std::get<N>(input);
                trf<T1,T2,N-1>::apply_pp(input, output);
            }
            static void apply_ind(T1& input, T2& output) {
                std::get<N>(output) = *std::get<N>(input);
                trf<T1,T2,N-1>::apply_ind(input, output);
            }
        };

        template <typename T1, typename T2>
        struct trf<T1,T2,0> {
            static void apply_pp(T1& input, T2& output) {
                std::get<0>(output) = ++std::get<0>(input);
            }
            static void apply_ind(T1& input, T2& output) {
                std::get<0>(output) = *std::get<0>(input);
            }
        };

        /*************COMPARE TUPLE***************/
        template <typename T, unsigned N = std::tuple_size<T>::value-1>
        struct cmp {
            static bool apply(const T& a, const T& b) {
                return cmp<T,N-1>::apply(a, b) && std::get<N>(a) == std::get<N>(b);
            }
        };

        template <typename T>
        struct cmp<T,0> {
            static bool apply(const T& a, const T& b) {
                return std::get<0>(a) == std::get<0>(b);
            }
        };

    }

    // iterator
    template <typename T, typename Value>
    struct ZipIterator : public std::iterator<std::input_iterator_tag, Value> {
        T source;
        using inner_type = T;
        using value_type = Value;

        ZipIterator(const T& s) : source(s) { }
        ZipIterator(T&& s) : source(std::move(s)) { }
        ZipIterator(ZipIterator&& o) : source(std::move(o.source)) { }
        ZipIterator(const ZipIterator& o) : source(o.source) { }
        ZipIterator& operator=(const ZipIterator& s) = delete;
        ZipIterator& operator=(ZipIterator&& s) = delete;

        bool operator== (const ZipIterator& o) const {
            return cmp<inner_type>::apply(source, o.source);
        }

        bool operator!= (const ZipIterator& o) const {
            return !cmp<inner_type>::apply(source, o.source);
        }

        ZipIterator operator++() {
            ZipIterator cpy = *this;
            trf<inner_type, inner_type, std::tuple_size<inner_type>::value-1>::apply_pp(source, source);
            return cpy;
        }

        ZipIterator& operator++(int) {
            trf<inner_type, inner_type, std::tuple_size<inner_type>::value-1>::apply_pp(source, source);
            return *this;
        }

        value_type operator*() {
            value_type vals;
            trf<inner_type, value_type, std::tuple_size<inner_type>::value-1>::apply_ind(source, vals);
            return vals;
        }

        value_type operator->() {
            value_type vals;
            trf<inner_type, value_type, std::tuple_size<inner_type>::value-1>::apply_ind(source, vals);
            return vals;
        }
    };


    // chainable
    template <typename... Args>
    struct zip_t {
        // tuple of given container value types
        using value_type = get_value_type_t<typename Args::value_type...>;
        std::tuple<Args&...> storage;

        using inner_iterator_type = get_value_type_t<typename Args::iterator...>;
        using iterator = ZipIterator<inner_iterator_type, value_type>;

        zip_t(Args&... containers) : storage(containers...) { }

        iterator begin() {
            return aux<iterator, decltype(storage), sizeof...(Args)>::get_begin(this->storage);
        }

        iterator end() {
            return aux<iterator, decltype(storage), sizeof...(Args)>::get_end(this->storage);
        }
    };

    // function
    /*
    template <typename... Args>
    zip_t<> zip(Args... a) {

    }
    */
}

