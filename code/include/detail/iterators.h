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

#include <type_traits>
#include <functional>

namespace func {
namespace it {
    template <typename V, typename S, typename F>
    struct TransformIterator;
    template <typename V, typename S, typename F>
    struct FilterIterator;
    template <typename V>
    struct SequenceIterator;
    template <typename T, typename V>
    struct ZipIterator;
    template<typename V, typename S, typename F>
    struct MuxIterator;
    template<typename V, typename S, typename F>
    struct DemuxIterator;
}
}

namespace std {

    //  Clang compiler runs into some trouble with the custom
    //  iterator traits, lets just define here some standard
    //  espetializations so everyone is happy

    template<typename V, typename S, typename F>
    struct iterator_traits<func::it::TransformIterator<V,S,F>>{
        using difference_type   = typename std::iterator_traits<S>::difference_type;
        using value_type        = V;
        using pointer           = const V*;
        using reference         = const V&;
        using iterator_category = std::input_iterator_tag;
    };

    template<typename V, typename S, typename F>
    struct iterator_traits<func::it::FilterIterator<V,S,F>>{
        using difference_type   = typename std::iterator_traits<S>::difference_type;
        using value_type        = V;
        using pointer           = const V*;
        using reference         = const V&;
        using iterator_category = std::input_iterator_tag;
    };

    template<typename V>
    struct iterator_traits<func::it::SequenceIterator<V>>{
        using difference_type   = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = V*;
        using reference         = V&;
        using iterator_category = std::input_iterator_tag;
    };

    template<typename I, typename V>
    struct iterator_traits<func::it::ZipIterator<I,V>>{
        using difference_type   =  std::ptrdiff_t;
        using value_type        = V;
        using pointer           = V*;
        using reference         = V&;
        using iterator_category = std::input_iterator_tag;
    };
    template<typename V, typename S, typename F>
    struct iterator_traits<func::it::MuxIterator<V,S,F>>{
        using difference_type   = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = const V*;
        using reference         = const V&;
        using iterator_category = std::input_iterator_tag;
    };
    template<typename V, typename S, typename F>
    struct iterator_traits<func::it::DemuxIterator<V,S,F>>{
        using difference_type   = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = const V*;
        using reference         = const V&;
        using iterator_category = std::input_iterator_tag;
    };
}


namespace func {
namespace detail {

    template <typename Iter>
    using is_ra_iterator = typename std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>;

    template <typename Iter, typename Value, typename Category>
    struct iterator_type_aux;

    template <typename Iter, typename Value>
    struct iterator_type : public std::iterator<std::input_iterator_tag, Value> {
        static const bool is_parallel_iterator = iterator_type_aux<
                                                            Iter,
                                                            Value,
                                                            typename std::iterator_traits<Iter>::iterator_category
                                                      >::is_parallel_iterator;
    };

    template <typename Value, typename Source, typename Func, typename Category>
    struct iterator_type_aux<it::TransformIterator<Value, Source, Func>, Value, Category> {
        static const bool is_parallel_iterator = iterator_type<Source, Value>::is_parallel_iterator;
    };

    template <typename Value, typename Source, typename Func, typename Category>
    struct iterator_type_aux<it::FilterIterator<Value, Source, Func>, Value, Category> {
        static const bool is_parallel_iterator = false;
    };

    template <typename Value, typename Category>
    struct iterator_type_aux<it::SequenceIterator<Value>, Value, Category> {
        static const bool is_parallel_iterator = true;
    };

    template <typename Value, typename Source, typename Func, typename Category>
    struct iterator_type_aux<it::MuxIterator<Value, Source, Func>, Value, Category> {
        static const bool is_parallel_iterator = false;
    };

    template <typename Value, typename Source, typename Func, typename Category>
    struct iterator_type_aux<it::DemuxIterator<Value, Source, Func>, Value, Category> {
        static const bool is_parallel_iterator = false;
    };

    template <typename Source, typename Value, typename Category>
    struct iterator_type_aux<it::ZipIterator<Source, Value>, Value, Category> {
        using first_iter = typename std::tuple_element<0,Source>::type;
        using first_value = typename std::tuple_element<0,Value>::type;
        using new_iter_tuple = typename detail::remove_first_type<Source>::type;
        using new_value_tuple = typename detail::remove_first_type<Value>::type;

        static const bool is_parallel_iterator = iterator_type<first_iter, first_value>::is_parallel_iterator &&
                    iterator_type<it::ZipIterator<new_iter_tuple, new_value_tuple>, new_value_tuple>::is_parallel_iterator;
    };

    template <typename Category>
    struct iterator_type_aux<it::ZipIterator<std::tuple<>, std::tuple<>>, std::tuple<>, Category> {
        static const bool is_parallel_iterator = true;
    };

    template <typename Iter, typename Value, typename Category>
    struct iterator_type_aux {
        static const bool is_parallel_iterator = std::is_same<Category, std::random_access_iterator_tag>();
    };

    template <typename Iter, typename Value>
    using iterator_type_t = typename iterator_type<Iter, Value>::type;

} // end namespace detail
} // end namespace func
