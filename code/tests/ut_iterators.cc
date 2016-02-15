#include "gtest/gtest.h"

#include <iterator>


template<typename T>
class my_iter: public std::iterator<std::forward_iterator_tag, T>{

    unsigned offset, size;
    T* buff;

    public:
        my_iter(): offset(0), size(0), buff(nullptr){}
        my_iter(int o, int s, T* buffer): offset(o), size(s), buff(buffer){}

        my_iter(const my_iter<T>& o): offset(o.offset), size(o.size), buff(o.buff) {}
        my_iter(my_iter<T>&& o): offset(o.offset), size(o.size), buff(o.buff) {}

        my_iter<T>&  operator += (unsigned i){
            offset+=i;
            if (offset > size) {
                offset = 0;
                buff = nullptr;
            }
            return *this;
        }
        my_iter<T>  operator + (unsigned i){
            my_iter<T> clone(*this);
            return clone+=i;
        }
        // prefix
        my_iter<T>  operator ++(){
            my_iter<T> clone(*this);
            return clone+=1;
        }
        // suffix
        my_iter<T>  operator ++(int){
            my_iter<T> clone(*this);
            *this+=1;
            return clone;
        }

        bool operator == (const my_iter<T>& o){
            return (o.buff == o.buff) && (o.offset == offset);
        }
        bool operator != (const my_iter<T>& o){
            return !(*this == o);
        }

        T& operator* (){
            return buff[offset];
        }
};

template<typename T, unsigned Size>
struct container{

    T buffer[Size];

    my_iter<T> begin(){
        return my_iter<T>(0,Size,buffer);
    }
    my_iter<T> end(){
        return my_iter<T>();
    }

    unsigned size(){
        return Size;
    }
};


TEST (Iterators, concept){

    container<int, 10> array;
    for (int i=0; i<10; ++i) array.buffer[i] = 11;

    for (auto it = array.begin(); it != array.end(); ++it){
        EXPECT_EQ(*it, 11);
    }

    std::vector<int> v(array.begin(), array.end());

    for( auto x :  v) 
        EXPECT_EQ(x , 11);
}
