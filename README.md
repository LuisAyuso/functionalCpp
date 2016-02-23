# functionalCpp

Chainable user defined operators for C++ collections. 
Wait, What does this mean? 

## Background 

Last time I had the chance of program some python I found myself writting long chains of _map_ / _ filter_  / _ reduce_  calls. 
Althoug a custom algorithm can perform much better, I find this intuitive and easy to program. Therefore this header-only library
is an attempt to implement chainable user defined operations for C++.

## What is implemented 

###Transform:   
    Transform each element of a collection, the result has the same arity, but might have different element type.
###Filter:          
    Filter elements based on custom criteria, the arity of the resulting collection can be 0 to the original size.
###Zip:          
    Merge N collections into a collection of agregates. If two collections, produces pairs, if more, produces tuples
###Reduce: 
    Reduce, compute some scalar value based on all elements in collection


## In the roadmap:

  + More awareness of the iterators, random acces iterator provided when available (Transform)
  + IO. Functional containers can manipulate infinite input streams. Lets implement some File and/or network sources.
  + Parallelism: When random access iterators are available, we can chunk it and process in parallel.
  + Pipelining: When there are no random access iterators, we can buffer part of the computation and pipeline it over the processors.

