//
//  filter.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#ifndef functional_filter_h
#define functional_filter_h
namespace func{
    
    template<typename Value, typename Source, typename Func>
    struct f_iterator{
        
        Func& f;
        Source s;
        Source end;
        
        using self_type = f_iterator<Value, Source, Func>;
        
        //TODO: standard says default constructed
        f_iterator(Func& f, const Source beg, const Source end)
        :f(f), s(beg), end(end) {
            while(s != end && !f(*s)){
                ++s;
            }
        }
        
        template <typename A, typename B, typename F>
        f_iterator(const f_iterator<A,B,F>& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        f_iterator(f_iterator<A,B,F>&& o)
        :f(o.f), s(o.s){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
        }
        
        template <typename A, typename B, typename F>
        bool operator == (const f_iterator<A,B,F>& o){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return this->s == o.s;
        }
        
        template <typename A, typename B, typename F>
        bool operator != (const f_iterator<A,B,F>& o){
            static_assert(std::is_same<A, Value>::value, "incompatible iterators");
            static_assert(std::is_same<B, Source>::value, "incompatible iterators");
            return s != o.s;
        }
        
        Value operator* (){
            return *s;
        }
        Value operator->(){
            return *s;
        }
        
        self_type& operator++(){
            do{
                ++s;
            }while(s != end && !f(*s));
            return *this;
        }
        self_type operator++(int){
            self_type cpy= *this;
            do{
                ++s;
            }while(s != end && !f(*s));
            return cpy;
        }
    };
    
    
    template<typename N, typename C>
    struct filter_t{
        
        C& storage;
        std::function<bool (N)>& func;
        
        using iterator = f_iterator<N, typename C::iterator, std::function<bool (N)>>;
        using const_iterator = const f_iterator<N, typename C::iterator, std::function<bool (N)>>;
        
        filter_t(std::function<bool (N)>& f, C& c)
        :storage(c), func(f){}
        
        iterator begin(){
            return iterator(func, storage.begin(), storage.end());
        }
        
        iterator end(){
            return iterator(func, storage.end(), storage.end());
        }
        
        const_iterator begin() const{
            return const_iterator(func, storage.begin());
        }
        const_iterator end() const{
            return const_iterator(func, storage.end());
        }
        
        operator std::vector<N>(){
            return std::vector<N>(begin(), end());
        }
    };
    
    template <typename N, typename C>
    filter_t<N,C> filter(std::function<bool (N)>& f, C& c){
        return filter_t<N,C>(f, c);
    }
    template <typename N,  typename C>
    filter_t<N,C> filter(std::function<bool (N)>& f, C&& c){
        return filter_t<N,C>(f, c);
    }
    
}



#endif
