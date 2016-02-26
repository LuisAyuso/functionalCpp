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
#include "detail/chaineable.h"

namespace func{
    namespace {

        /*************GET TUPLE TYPES***************/

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
            using type = std::pair<T, V>;
        };

        template <typename... Args>
        using get_value_type_t = typename get_value_type<Args...>::type;

        /************ Turn tuple of storages into tuple of Value types ********************/

        template <typename T, unsigned N = std::tuple_size<T>::value, typename... Args>
        struct unstore{
            using type = typename unstore<T, N-1, typename std::tuple_element<N-1, T>::type::stored_type, Args...>::type;
        };
        template <typename T, typename... Args>
        struct unstore<T, 0, Args...>{
            using type = get_value_type_t<typename Args::value_type...>;
        };

        template <typename T>
        using unstore_t = typename unstore<T>::type;

        /*************GET TUPLE ELEMENT***************/

        template <typename Iter, typename T, unsigned N>
        struct aux {
            template <typename... Args>
            static Iter get_begin(T& tuple, Args&... storages) {
                return aux<Iter,T,N-1>::get_begin(tuple, std::get<N-1>(tuple), storages...);
            }
            template <typename... Args>
            static Iter get_end(T& tuple, Args&... storages) {
                return aux<Iter,T,N-1>::get_end(tuple, std::get<N-1>(tuple), storages...);
            }
        };

        template <typename Iter, typename T>
        struct aux<Iter,T,0> {
            template <typename... Args>
            static Iter get_begin(T& tuple, Args&... storages) {
                return Iter(typename Iter::inner_type(storages->begin()...), typename Iter::inner_type(storages->end()...));
            }
            template <typename... Args>
            static Iter get_end(T& tuple, Args&... storages) {
                return Iter(typename Iter::inner_type(storages->end()...), typename Iter::inner_type(storages->end()...));
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

        template <typename T>
        void plusplus (T& source, T& target){
            trf<T, T, std::tuple_size<T>::value-1>::apply_pp(source, target);
        }
        template <typename T, typename V>
        void asterisk (T& source, V& target){
            trf<T, V, std::tuple_size<T>::value-1>::apply_ind(source, target);
        }

        /**************** Check if tuple reaches end ******************/

        template <typename T, unsigned  N>
        struct check{
            static bool eq (const T& begin, const T& end){
                return std::get<N>(begin) == std::get<N>(end) || check<T, N-1>::eq(begin, end);
            }
        };
        template <typename T>
        struct check<T, 0>{
            static bool eq (const T& begin, const T& end){
                return std::get<0>(begin) == std::get<0>(end);
            }
        };

        template <typename T>
        bool is_end(const T& begin, const T& end){
            return check<T, std::tuple_size<T>::value -1>::eq(begin, end);
        }
    }

    // iterator
    template <typename T, typename Value>
    struct ZipIterator : public std::iterator<std::input_iterator_tag, Value> {
        T source, end;
        bool finish;
        using inner_type = T;
        using value_type = Value;

        ZipIterator(): finish(true) {}
        ZipIterator(const T& source, const T& end) : source(source), end(end), finish(is_end(source, end)) { }

        ZipIterator(const ZipIterator& o) : source(o.source), end(o.end), finish(o.finish) { }
        ZipIterator(ZipIterator&& o) : source(std::move(o.source)), end(std::move(o.end)), finish(o.finish) { }

        ZipIterator& operator=(const ZipIterator& o){
            source = o.source;
            end = o.end;
            finish = o.finish;
        };
        ZipIterator& operator=(ZipIterator&& o) {
            std::swap(source, o.source);
            std::swap(end, o.end);
            finish = o.finish;
        }

        bool operator== (const ZipIterator& o) const {
           return finish == o.finish || source == o.source;
        }

        bool operator!= (const ZipIterator& o) const {
           return !(*this == o);
        }

        ZipIterator& operator++() {
            plusplus(source, source);
            finish = is_end(source, end);
            return *this;
        }

        ZipIterator operator++(int) {
            ZipIterator cpy = *this;
            plusplus(source, source);
            finish = is_end(source, end);
            return cpy;
        }

        value_type operator*() {
            assert(!finish && "can not deref end iterator");
            value_type vals;
            asterisk(source, vals);
            return vals;
        }
    };


    // chainable
    template <typename... Args>
    struct zip_t {

        // this tuple contains the storage of the input containers
        std::tuple<Args...> storage;

        using value_type = get_value_type_t<typename Args::stored_type::value_type...>;
        using inner_iterator_type = get_value_type_t<typename Args::stored_type::iterator...>;
        using iterator = ZipIterator<inner_iterator_type, value_type>;

        /* 
         * it only accepts rvalues to intialize, 
         * a helper function needs to be used to construct the containers and pass them
        */
        zip_t(Args&&... containers) : storage(std::forward<Args>(containers)...) { }

        iterator begin() {
            return aux<iterator, decltype(storage), sizeof...(Args)>::get_begin(this->storage);
        }

        iterator end() {
            return aux<iterator, decltype(storage), sizeof...(Args)>::get_end(this->storage);
        }
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
    template <typename A, typename B>
    zip_t<detail::ref_t<A>, detail::ref_t<B>> zip(A& a, B& b){
        using namespace detail;
        return zip_t<ref_t<A>,ref_t<B>> (ref_t<A>(a), ref_t<B>(b));
    }

    template <typename A, typename B>
    zip_t<detail::val_t<A>, detail::ref_t<B>> zip(A&& a, B& b){
        using namespace detail;
        return zip_t<val_t<A>,ref_t<B>> (val_t<A>(std::move(a)), ref_t<B>(b));
    }

    template <typename A, typename B>
    zip_t<detail::ref_t<A>, detail::val_t<B>> zip(A& a, B&& b){
        using namespace detail;
        return zip_t<ref_t<A>,val_t<B>> (ref_t<A>(a), val_t<B>(std::move(b)));
    }

    template <typename A, typename B>
    zip_t<detail::val_t<A>, detail::val_t<B>> zip(A&& a, B&& b){
        using namespace detail;
        return zip_t<val_t<A>,val_t<B>> (val_t<A>(std::move(a)), val_t<B>(std::move(b)));
    }
}

