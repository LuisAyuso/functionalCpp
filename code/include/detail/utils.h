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
namespace detail {

    template <class T>
    struct remove_all {
        using type = typename std::remove_reference<typename std::remove_cv<T>::type>::type;
    };

    template<class T>
    using remove_all_t = typename remove_all<T>::type;

    template <class T>
    struct symplify_pair {
        using type = T;
    };

    template <class A, class B>
    struct symplify_pair<std::pair<A, B>>{
        using type = std::pair<remove_all_t<A>, remove_all_t<B>>;
    };

    template<class... T>
    using symplify_pair_t = typename symplify_pair<T...>::type;

    template<class T>
    using remove_reference_t = typename std::remove_reference<T>::type;

    template<class T>
    using remove_cv_t = typename std::remove_cv<T>::type;

    template <typename N, typename M>
    struct equal_arg_type {
        static constexpr bool value = std::is_same<
            remove_reference_t<N>,
            remove_reference_t<M>
        >::value;
    };


    template<typename C>
    struct get_collection_type{
        using type = symplify_pair_t<typename remove_all_t<C>::value_type>;
    };

    template<typename N>
    struct remove_cv_aux{
        using type = typename std::remove_cv<N>::type;
    };

    // when having a lambda with a pair, we can const both memebers
    // since we can allways call a more restrictive function
    template<typename N, typename M>
    struct remove_cv_aux<std::pair<N,M>>{
        using type = typename std::pair<
                        remove_cv_t<N>,
                        remove_cv_t<M>
                    >;
    };

    template <typename N, typename C>
    struct validate_container_function{
        static constexpr bool value = equal_arg_type<
                                typename remove_cv_aux<N>::type,
                                typename get_collection_type<C>::type
                            >::value;
    };



    template <typename F, typename VT>
    struct get_ret_type_for_lambda{
        static VT dummy_value;
        static F dummy_f;
        using ret_type = decltype(dummy_f(dummy_value));
    };

//    template <typename F, typename C>
//    void test(F f, C c){
//
//        typename get_ret_type_for_lambda<F, C>::ret_type x;
//        static_assert(std::is_same<decltype(x), typename func::get_collection_type<C>::type>::value, "yo");
//    };

    // notes: get the return value, use a constexpr and call the function, the expression should
    // have the a return type of the lambda
    template <typename F, typename C>
    struct get_lambda{
    private:
        using collection_type = typename get_collection_type<C>::type;
    public:
        using param_type = collection_type;
        using return_type = typename get_ret_type_for_lambda<F, collection_type>::ret_type;
    };



    template <typename N, typename C>
    struct get_lambda<std::function<N(N)>, C>{
        using param_type = N;
        using return_type = N;
    };

    template <typename N, typename R, typename C>
    struct get_lambda<std::function<R(N)>, C>{
        using param_type = N;
        using return_type = R;
    };


    //check if an iterator is a random access iterator
    template <typename Container>
    using has_ra_iter = typename std::is_same<typename std::iterator_traits<Container>::iterator_category, std::random_access_iterator_tag>;

    //evaluate has_ra_iter<Container>
    template <typename Container>
    using has_ra_iter_v = typename has_ra_iter<Container>::value;

    template <typename V, typename T, bool = has_ra_iter<T>::value>
    struct is_ra_iterable;

    //non random access iterators will derive from this struct
    template <typename V, typename T>
    struct is_ra_iterable<V,T,false> : public std::iterator<std::input_iterator_tag, V> {};

    //random access iterators will derive from this struct
    template <typename V, typename T>
    struct is_ra_iterable<V,T,true> : public std::iterator<std::bidirectional_iterator_tag, V> {

    };

 } // end namespace detail
 } // end namespace func
