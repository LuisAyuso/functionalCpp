#include "gtest/gtest.h"

#include <iterator>
#include <vector>
#include <list>
#include <map>
#include <array>

#include "utils.h"

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

TEST (Iterators, random_access) {
    std::vector<int> v = {1,2,3};
    std::list<int> l = {1,2,3};
    std::array<int,3> a = {1,2,3};
    std::map<int,int> m = {{1,2},{2,3},{3,4}};

    //Test if it is possible to identify some container as random iterable or not
    EXPECT_TRUE(func::has_ra_iter<decltype(v)::iterator>::value);
    EXPECT_TRUE((func::has_ra_iter<decltype(a)::iterator>::value));
    EXPECT_FALSE(func::has_ra_iter<decltype(l)::iterator>::value);
    EXPECT_FALSE((func::has_ra_iter<decltype(m)::iterator>::value));

    //We can derive from is_ra_iterable<Container> metafunction in order to 
    //automatically derive from different classes for random or non random iterable containers
    //test vector
    EXPECT_TRUE((std::is_base_of<std::iterator<std::bidirectional_iterator_tag, int>,
                                 typename func::is_ra_iterable<int,decltype(v)::iterator>
                                 >::value));
    EXPECT_FALSE((std::is_base_of<std::iterator<std::input_iterator_tag, int>,
                                 typename func::is_ra_iterable<int,decltype(v)::iterator>
                                 >::value));
    //test array
    EXPECT_TRUE((std::is_base_of<std::iterator<std::bidirectional_iterator_tag, int>,
                                 typename func::is_ra_iterable<int,decltype(a)::iterator>
                                 >::value));
    EXPECT_FALSE((std::is_base_of<std::iterator<std::input_iterator_tag, int>,
                                 typename func::is_ra_iterable<int,decltype(a)::iterator>
                                 >::value));

    //list array
    EXPECT_FALSE((std::is_base_of<std::iterator<std::bidirectional_iterator_tag, int>,
                                 typename func::is_ra_iterable<int,decltype(l)::iterator>
                                 >::value));
    EXPECT_TRUE((std::is_base_of<std::iterator<std::input_iterator_tag, int>,
                                 typename func::is_ra_iterable<int,decltype(l)::iterator>
                                 >::value));

    //map array
    EXPECT_FALSE((std::is_base_of<std::iterator<std::bidirectional_iterator_tag, std::pair<int,int>>,
                                 typename func::is_ra_iterable<std::pair<int,int>,decltype(m)::iterator>
                                 >::value));
    EXPECT_TRUE((std::is_base_of<std::iterator<std::input_iterator_tag, std::pair<int,int>>,
                                 typename func::is_ra_iterable<std::pair<int,int>,decltype(m)::iterator>
                                 >::value));

}
