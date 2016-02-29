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
#include "detail/iterators.h"
#include "detail/chaineable.h"

namespace func{

    template<typename Value, typename Source, typename Func>
    struct TransformIterator : public detail::iterator_type<TransformIterator<Value, Source, Func>, Value> {

        Func& f;
        Source s;

        using source_type = Source;
        using self_type = TransformIterator<Value, Source, Func>;

        TransformIterator(Func& f, const Source s, const Source)
        :f(f), s(s) {}

        TransformIterator(const TransformIterator& o)
        : f(o.f), s(o.s)
        {}

        TransformIterator(TransformIterator&& o)
        : f(o.f), s(std::move(o.s))
        {}

        TransformIterator& operator= (const TransformIterator& o){
            s = o.s;
            return *this;
        }

        TransformIterator& operator= (TransformIterator&& o){
            std::swap(s, o.s);
            return *this;
        }

        template <typename A, typename B, typename F>
        TransformIterator(const TransformIterator<A,B,F>& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }

        template <typename A, typename B, typename F>
        TransformIterator(TransformIterator<A,B,F>&& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }

        template <typename A, typename B, typename F>
        bool operator == (const TransformIterator<A,B,F>& o) const{
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return this->s == o.s;
        }

        template <typename A, typename B, typename F>
        bool operator != (const TransformIterator<A,B,F>& o) const{
            static_assert(std::is_same<A, Value>::value,  "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return s != o.s;
        }

        Value operator* (){
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

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        Value operator[](unsigned i) {
            return f(s[i]);
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        S operator+ (int i) {
            return S(f, s+i, s);
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        S operator- (int i) {
            return S(f, s-i, s);
        }

        template <typename S = self_type, typename = typename std::enable_if<S::is_parallel_iterator>::type>
        typename std::iterator_traits<S>::difference_type operator- (const S& o) {
            return s-o.s;
        }

    };


   template <typename FuncType, typename C, typename Storage_type>
    using transform_t = detail::chaineable_t<
                            FuncType, C, Storage_type, // forward paramenters
                            TransformIterator<typename detail::get_lambda<FuncType,C>::return_type, typename C::iterator, FuncType > // specific iterator type for transformation
                                >;


    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    transform_t<std::function<R (N)>,C,detail::Reference_storage>
    transform(std::function<R (N)> f, C& c){
        return transform_t<std::function<R(N)>,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    transform_t<std::function<R(N)>,C,detail::Value_storage>
    transform(std::function<R (N)> f, C&& c){
        return transform_t<std::function<R(N)>,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename C>
    transform_t<F, C, detail::Reference_storage>
    transform(F f, C& c){
        return transform_t<F,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename C>
    transform_t<F, C, detail::Value_storage>
    transform(F f, C&& c){
        return transform_t<F,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }
}

