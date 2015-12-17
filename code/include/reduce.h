//
//  reduce.h
//  functional
//
//  Created by Luis Ayuso on 18/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#ifndef functional_reduce_h
#define functional_reduce_h


namespace func{
    
    template <typename N, typename C>
    N reduce_aux(std::function<N (N, N)>& f, C& c){
        auto it = c.begin();
        if (it != c.end()){
            
            N value = *it;
            ++it;
            
            while (it != c.end()){
                value = f(value, *it);
                ++it;
            }
            return value;
        }
        // todo, drop an exception or something?
        return N();
    }

    
    template <typename N, typename C>
    N reduce(std::function<N (N, N)>& f, C& c){ return reduce_aux(f, c);}

    template <typename N, typename C>
    N reduce(std::function<N (N, N)>& f, C&& c){ return reduce_aux(f, c);}

}

#endif
