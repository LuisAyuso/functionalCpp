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
#include "gtest/gtest.h"

#include <iterator>
#include <vector>
#include <list>
#include <map>
#include <array>

#include "detail/utils.h"
#include "detail/iterators.h"
#include "transform.h"
#include "filter.h"
#include "zip.h"

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

TEST (Iterators, parallel_iterator) {
    using p = std::pair<int,int>;
    using t = std::tuple<int,float,bool>;

    EXPECT_EQ((std::is_same<func::detail::remove_first_type<p>::type, std::tuple<int>>::value), true);
    EXPECT_EQ((std::is_same<func::detail::remove_first_type<func::detail::remove_first_type<p>::type>::type, std::tuple<>>::value), true);

    EXPECT_EQ((std::is_same<func::detail::remove_first_type<t>::type, std::tuple<float,bool>>::value), true);
    EXPECT_EQ((std::is_same<func::detail::remove_first_type<func::detail::remove_first_type<t>::type>::type, std::tuple<bool>>::value), true);


    using vec = std::vector<int>;
    using list = std::list<int>;
    ASSERT_TRUE( (std::is_same< vec::iterator::iterator_category, std::random_access_iterator_tag>()) );

    using transform_par = func::it::TransformIterator<int,vec::iterator,std::function<int(int)>>;
    using transform_no_par = func::it::TransformIterator<int,list::iterator,std::function<int(int)>>;

    ASSERT_TRUE( (std::is_same< transform_par::iterator_category, std::input_iterator_tag>()) );
    ASSERT_TRUE( (std::is_same< transform_par::iterator::iterator_category, std::input_iterator_tag>()) );
    ASSERT_TRUE( (std::is_same< std::iterator_traits<transform_par>::iterator_category, std::input_iterator_tag>()) );

    ASSERT_TRUE(transform_par::is_parallel_iterator);
    ASSERT_FALSE(transform_no_par::is_parallel_iterator);

    using filter_vec = func::it::FilterIterator<int,vec::iterator,std::function<int(int)>>;
    using filter_list = func::it::FilterIterator<int,list::iterator,std::function<int(int)>>;
    ASSERT_FALSE(filter_vec::is_parallel_iterator);
    ASSERT_FALSE(filter_list::is_parallel_iterator);

    using nested_par = func::it::TransformIterator<int,transform_par,std::function<int(int)>>;
    using nested_no_par = func::it::TransformIterator<int,transform_no_par,std::function<int(int)>>;
    ASSERT_TRUE(nested_par::is_parallel_iterator);
    ASSERT_FALSE(nested_no_par::is_parallel_iterator);

    using filter_trans = func::it::FilterIterator<int,transform_par,std::function<int(int)>>;
    using trans_filter = func::it::TransformIterator<int,filter_vec,std::function<int(int)>>;
    ASSERT_FALSE(filter_trans::is_parallel_iterator);
    ASSERT_FALSE(trans_filter::is_parallel_iterator);

    using zip_par = func::it::ZipIterator<std::pair<vec::iterator,vec::iterator>, std::pair<int,int>>;
    using zip_par_one = func::it::ZipIterator<std::pair<transform_par,transform_par>, std::pair<int,int>>;
    using zip_par_two = func::it::ZipIterator<std::pair<transform_par,filter_vec>, std::pair<int,int>>;
    using zip_no_par = func::it::ZipIterator<std::pair<vec::iterator,list::iterator>, std::pair<int,int>>;
    using zip_no_par_one = func::it::ZipIterator<std::pair<list::iterator,vec::iterator>, std::pair<int,int>>;
    using zip_no_par_two = func::it::ZipIterator<std::pair<list::iterator,list::iterator>, std::pair<int,int>>;
    ASSERT_TRUE(zip_par::is_parallel_iterator);
    ASSERT_TRUE(zip_par_one::is_parallel_iterator);
    ASSERT_FALSE(zip_par_two::is_parallel_iterator);
    ASSERT_FALSE(zip_no_par::is_parallel_iterator);
    ASSERT_FALSE(zip_no_par_one::is_parallel_iterator);
    ASSERT_FALSE(zip_no_par_two::is_parallel_iterator);

}
