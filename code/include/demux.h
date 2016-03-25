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
    namespace {
        template <typename F, typename Param>
        struct get_ret_type{
            static Param* dummy;
            static F dummyF;
            using type = decltype(dummyF(*dummy));
        };

        template <typename A, typename B>
        struct validate{
        static_assert(std::is_same<A, B>(),
                    "the internal collection should store the same value");
        static_assert(std::is_same<A, B>(),
                    "the internal collection should store the same value");
        };
    }

    template<typename Value, typename Source, typename Func>
    struct DemuxIterator : public detail::iterator_type<DemuxIterator<Value, Source, Func>, Value> {

        Func& f;
        Source s, e;

        using local_collection = typename get_ret_type<Func, Source>::type;
        using local_it = typename local_collection::iterator;

        validate<typename local_collection::value_type, Value> _a;
        validate<typename local_it::value_type, Value> _b;

        local_collection last_result;
        local_it local_s, local_e;

        using source_type = Source;
        using self_type = DemuxIterator<Value, Source, Func>;

        DemuxIterator(Func& f, const Source& s, const Source& e)
        :f(f), s(s), e(e){
            if (s!=e){
                last_result = this->f(*s);
                this->s++;
            }
            local_s = last_result.begin();
            local_e = last_result.end();
            if (s==e) assert(local_e == local_s);
            if (s==e) assert(local_e - local_s == 0);
            if (s==e) assert(last_result.empty());
        }

        DemuxIterator(const DemuxIterator& o)
        : f(o.f), s(o.s), e(o.e),
          last_result(o.last_result), local_e(o.local_e), local_s(o.local_s)
        {}

        DemuxIterator(DemuxIterator&& o)
        : f(o.f), s(std::move(o.s)), e(std::move(o.e)),
          last_result(std::move(o.last_result)),
          local_e(std::move(o.local_e)), local_s(std::move(o.local_s))
        {}

        DemuxIterator& operator= (const DemuxIterator& o){
            s = o.s;
            last_result = o.last_result;
            local_s = o.local_s;
            local_e = o.local_e;
            return *this;
        }

        DemuxIterator& operator= (DemuxIterator&& o){
            std::swap(s, o.s);
            std::swap(last_result, o.last_result);
            std::swap(local_s, o.local_s);
            std::swap(local_e, o.local_e);
            return *this;
        }

        bool operator == (const DemuxIterator& o) const{
            // check whenever we have some work in progress.
            auto diff_me = local_e - local_s;
            auto diff_o = o.local_e - o.local_s;
            return (diff_me == diff_o) && (s == o.s);
        }

        bool operator != (const DemuxIterator& o) const{
            return !(*this == o);
        }

        Value operator* () const{
            return *local_s;
        }

        const Value* operator-> () const{
            return &(*(local_s));
        }

        void plusplus(){
            if (local_e != local_s){
                local_s++;
            }
            if (local_s == local_e && s != e){
                assert(s != e);
                static_assert(std::is_same<decltype(last_result), decltype(f(*s))>(), "must be eq");
                last_result=f(*s);
                local_s = last_result.begin();
                local_e = last_result.end();
                s++;
            }
        }

        self_type& operator++(){
            plusplus();
            return *this;
        }

        self_type operator++(int){
            self_type cpy= *this;
            plusplus();
            return cpy;
        }
    };
} // it namespace

   template <typename FuncType, typename Container, typename Storage_type>
    using demux_t = detail::chaineable_t<
                            FuncType,
                            Container,
                            Storage_type,
                            typename detail::get_lambda<FuncType,Container>::return_type::value_type,
                            it::DemuxIterator<typename detail::get_lambda<FuncType,Container>::return_type::value_type, typename Container::iterator, FuncType > // specific iterator type for demuxation
                                >;


    // for function type
    // lvalue collection
    template <typename N, typename R, typename C>
    demux_t<std::function<R (N)>,C,detail::Reference_storage>
    demux(std::function<R (N)> f, C& c){
        return demux_t<std::function<R(N)>,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for function type
    // rvalue collection
    template <typename N, typename R, typename C>
    demux_t<std::function<R(N)>,C,detail::Value_storage>
    demux(std::function<R (N)> f, C&& c){
        return demux_t<std::function<R(N)>,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }

    // for lambda type
    // lvalue collection
    template <typename F, typename C>
    demux_t<F, C, detail::Reference_storage>
    demux(F f, C& c){
        return demux_t<F,C,detail::Reference_storage> (f, detail::chaineable_store_t<C,detail::Reference_storage> (c));
    }

    // for lambda type
    // xvalue collection
    template <typename F, typename C>
    demux_t<F, C, detail::Value_storage>
    demux(F f, C&& c){
        return demux_t<F,C,detail::Value_storage> (f, detail::chaineable_store_t<C,detail::Value_storage> (std::move(c)));
    }
}
