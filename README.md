# functionalCpp

Chainable user defined operators for C++ collections.
Wait, What does this mean?

## Background

Not so long time ago, I had the chance of program some python I found myself writing long chains of _map_ / _filter_  / _reduce_  calls.
Although a custom algorithm can perform much better, I find functional chains intuitive and easy to program. Therefore this header-only library
is an attempt to implement chainable user defined operations for C++.

## What is implemented

###Transform:   
Transform each element of a collection, the result has the same arity, but might have different element type.
###Filter:          
Filter elements based on custom criteria, the arity of the resulting collection can be 0 to the original size.
###Generator:          
Currently there is a sequence generator, defined by a start value and step.
###Zip:          
Merge N collections into a collection of aggregates. If two collections, produces a collection of pairs, if more, produces collection a of tuples.
###Reduce:
Reduce, compute some scalar value based on all elements in collection.

# mux / demux
The *mux* operation converts a series of elements in the input into a single output.
The *demux* operation converts a single element from the input into a series of output elements.

## Parallel iterators:

We are in the era of multicores! Lets do some multiprocessing.
Traditional parallel loops require of random access iterators to: first, compute the number of iterations, and second,  access elements with a certain offset.
Although we can provide (in some scenarios) this functionality. We can no guarantee all the requirements of standard iterators traits (see: [standard](http://en.cppreference.com/w/cpp/concept/RandomAccessIterator) ).

For this reason iterators will be marked as parallel iterators when possible:

| Operation     | Parallel?                                                      |
| ------------- |:--------------------------------------------------------------:|
| Transform     | **Yes** if nested collection can be accessed in parallel       |
| Filter        | **No**, never                                                  |
| Zip           | **Yes**, if all nested collections can be accessed in parallel |
| Sequence      | **Yes**, why not?                                              |
| Mux / Demux   | **Just dont think so**                                         |

A collection will be accessed in parallel whenever it provides random access iterators:

A parallel iterator provides extra methods
- operator +
- operator - (for scalar and for iterator difference)
- operator []

## In the roadmap:

  + ~~More awareness of the iterators, random access iterator provided when available (Transform)~~
  + IO. Functional containers can manipulate infinite input streams. Lets implement some File and/or network sources.
  + Parallelism: When ~~random~~ (parallel) access iterators are available, we can chunk it and process in parallel.
  + Pipelining: When there are no random access iterators, we can buffer part of the computation and pipeline it over the processors.
  + OutputIterators: Could we use a filter iterator as left side of an assignment?

## License

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
