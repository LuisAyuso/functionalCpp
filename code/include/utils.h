//
//  utils.h
//  functional
//
//  Created by Luis Ayuso on 09/03/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#ifndef functional_utils_h
#define functional_utils_h

#include <map>

#include <type_traits>

namespace func {
    
    template <class T>
    struct remove_all{
    using type = typename std::remove_reference<typename std::remove_cv<T>::type>::type;
    };
    
    
    template <class T>
    struct symplify_pair{
        using type = T;
    };
    
    template <class A, class B>
    struct symplify_pair<std::pair<A, B>>{
        using type = std::pair<typename remove_all<A>::type, typename remove_all<B>::type>;
    };
    
    template <typename N, typename M>
    struct equal_arg_type{

    // DEBUG:
//        static_assert(N::dummy_error, "DUMP MY TYPE" );
//        static_assert(M::dummy_error, "DUMP MY TYPE" );
   
        static constexpr bool value = std::is_same<
                    typename std::remove_reference<N>::type,
                    typename std::remove_reference<M>::type
                >::value;
    };
    
    template<typename C>
    struct get_collection_type{
        using pure_type = typename remove_all<C>::type;
        using type = typename symplify_pair<typename pure_type::value_type>::type;
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
                        typename std::remove_cv<N>::type,
                        typename std::remove_cv<M>::type
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
     
    
 } // end namespace


#endif
