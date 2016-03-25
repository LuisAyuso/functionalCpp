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
#include <algorithm>

#include "detail/utils.h"
#include "detail/iterators.h"
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
            static void apply_plus(T1& input, T2& output, int i) {
                std::get<N>(output) = std::get<N>(input) + i;
                trf<T1,T2,N-1>::apply_plus(input, output, i);
            }
            static void apply_minus(T1& input, T2& output, int i) {
                std::get<N>(output) = std::get<N>(input) - i;
                trf<T1,T2,N-1>::apply_minus(input, output, i);
            }
            static void apply_bracket(T1& input, T2& output, int i) {
                std::get<N>(output) = std::get<N>(input)[i];
                trf<T1,T2,N-1>::apply_bracket(input, output, i);
            }
            static void apply_sizes(const T1& iter_input_1, const T1& iter_input_2, T2& target) {
                target[N] = std::get<N>(iter_input_1)-std::get<N>(iter_input_2);
                trf<T1,T2,N-1>::apply_sizes(iter_input_1, iter_input_2, target);
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
            static void apply_plus(T1& input, T2& output, int i) {
                std::get<0>(output) = std::get<0>(input) + i;
            }
            static void apply_minus(T1& input, T2& output, int i) {
                std::get<0>(output) = std::get<0>(input) - i;
            }
            static void apply_bracket(T1& input, T2& output, int i) {
                std::get<0>(output) = std::get<0>(input)[i];
            }
            static void apply_sizes(const T1& iter_input_1, const T1& iter_input_2, T2& target) {
                target[0] = std::get<0>(iter_input_1)-std::get<0>(iter_input_2);
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
        template <typename T, typename V>
        void plus (T& source, V& target, int i){
            trf<T, V, std::tuple_size<T>::value-1>::apply_plus(source, target, i);
        }
        template <typename T, typename V>
        void minus (T& source, V& target, int i){
            trf<T, V, std::tuple_size<T>::value-1>::apply_minus(source, target, i);
        }
        template <typename T, typename V>
        void bracket (T& source, V& target, int i){
            trf<T, V, std::tuple_size<T>::value-1>::apply_bracket(source, target, i);
        }
        template <typename T, typename V>
        void sizes (const T& iter_input_1, const T& iter_input_2, V& target) {
            trf<T, V, std::tuple_size<T>::value-1>::apply_sizes(iter_input_1, iter_input_2, target);
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

namespace it{
    // iterator
    template <typename T, typename Value>
    struct ZipIterator : public detail::iterator_type<ZipIterator<T,Value>, Value> {
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

        template <typename S = ZipIterator, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        value_type operator[](unsigned i) {
            value_type x;
            bracket(source, x, i);
            return x;
        }

        template <typename S = ZipIterator, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        S operator+ (int i) {
            //apply operator+ on whole tuple
            S cpy = *this;
            plus(source, cpy.source, i);
            cpy.finish = is_end(cpy.source, cpy.end);
            return cpy;
        }

        template <typename S = ZipIterator, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        S operator- (int i) {
            //apply operator- on whole tuple
            S cpy = *this;
            minus(source, cpy.source, i);
            cpy.finish = is_end(cpy.source, cpy.end);
            return cpy;
        }

        template <typename S = ZipIterator, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        typename std::iterator_traits<S>::difference_type operator- (const S& o) {
            // get the smallest possible distance
            std::array<unsigned, std::tuple_size<T>::value> s;
            sizes(source, o.source, s);
            return *std::min_element(s.begin(),s.end());
        }
    };

} // namespace iterator

    // chainable
    template <typename... Args>
    struct zip_t {

        // this tuple contains the storage of the input containers
        std::tuple<Args...> storage;

        using value_type = get_value_type_t<typename Args::stored_type::value_type...>;
        using inner_iterator_type = get_value_type_t<typename Args::stored_type::iterator...>;
        using iterator = it::ZipIterator<inner_iterator_type, value_type>;

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

namespace{
    template <typename T>
    constexpr detail::val_t<T> get_storage(T&& v) {
        return detail::val_t<T>(std::move(v));
    }

    template <typename T>
    constexpr detail::ref_t<T> get_storage(T& v) {
        return detail::ref_t<T>(v);
    }
}
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    template <typename... Args>
    zip_t<detail::choose_storage_t<func::detail::get_reference_t<Args>>...> zip (Args&&... a) {
        using namespace func::detail;
        return zip_t<detail::choose_storage_t<get_reference_t<Args>>...> (get_storage(std::forward<Args>(a))...);
    }

}
