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
namespace it{

    template<typename Value, typename Source, typename Func>
    struct MuxIterator : public detail::iterator_type<MuxIterator<Value, Source, Func>, Value> {

        Func& f;
        Source s, e;
        Value last_value;
        mutable bool pre_end;

        using source_type = Source;
        using self_type = MuxIterator<Value, Source, Func>;

        MuxIterator(Func& f, const Source& s, const Source& e)
        :f(f), s(s), e(e), pre_end(false){
            if(s == e)  return;
            last_value = f(this->s, this->e);
            pre_end = (this->s == this->e);
        }

        MuxIterator(const MuxIterator& o)
        : f(o.f), s(o.s), e(o.e), last_value(o.last_value), pre_end(o.pre_end)
        {}

        MuxIterator(MuxIterator&& o)
        : f(o.f), s(std::move(o.s)), e(std::move(o.e)),
          last_value(std::move(o.last_value)), pre_end(o.pre_end)
        {}

        MuxIterator& operator= (const MuxIterator& o){
            s = o.s;
            last_value = o.last_value;
            pre_end = o.pre_end;
            return *this;
        }

        MuxIterator& operator= (MuxIterator&& o){
            std::swap(s, o.s);
            std::swap(last_value, o.last_value);
            std::swap(pre_end, o.pre_end);
            return *this;
        }

        bool operator == (const MuxIterator& o) const{
            return  (s == o.s) && (pre_end == o.pre_end);
        }

        bool operator != (const MuxIterator& o) const{
            return !(*this == o);
        }

        Value operator* () const{
            return last_value;
        }

        const Value* operator-> () const{
            return &last_value;
        }

        self_type& operator++(){
            if (pre_end) {
                assert(s == e);
                pre_end = false;
                return *this;
            }
            last_value = f(s,e);
            pre_end = s == e;
            return *this;
        }

        self_type operator++(int){
            self_type cpy= *this;
            if (pre_end) {
                assert(s == e);
                pre_end = false;
                return cpy;
            }
            last_value = f(s,e);
            pre_end = s == e;
            return cpy;
        }
    };
} // it namespace

   template <typename FuncType, typename Container, typename Storage_type>
    using mux_t = detail::chaineable_t<
                            FuncType,
                            Container,
                            Storage_type,
                            typename detail::get_lambda_iterator<FuncType,Container>::return_type,
                            it::MuxIterator<typename detail::get_lambda_iterator<FuncType,Container>::return_type, typename Container::iterator, FuncType > // specific iterator type for muxation
                                >;


    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    mux_t<std::function<R (N)>,C,detail::Reference_storage>
    mux(std::function<R (N)> f, C& c){
        return mux_t<std::function<R(N)>,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    mux_t<std::function<R(N)>,C,detail::Value_storage>
    mux(std::function<R (N)> f, C&& c){
        return mux_t<std::function<R(N)>,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename C>
    mux_t<F, C, detail::Reference_storage>
    mux(F f, C& c){
        return mux_t<F,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename C>
    mux_t<F, C, detail::Value_storage>
    mux(F f, C&& c){
        return mux_t<F,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }
}
