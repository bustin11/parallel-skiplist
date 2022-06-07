# Requirements

## Hardware
- Tested on debian-like machine (pop-os to be exact), but ubuntu probably works. CPU information can be found https://ark.intel.com/content/www/us/en/ark/products/208658/intel-core-i51135g7-processor-8m-cache-up-to-4-20-ghz.html. Main point is that I have 4 cores, but can run 8 threads ("simultaneously") since intel supports hardware threads.


## Software
- Tested with g++ --version=10.3.0
```
sudo apt-get install g++ // or
sudo apt-get build-essential
```
-  Install **openMP**
```
sudo apt-get install libomp-dev
```

**openMP** is a library that offers support for seamless parallelization through thread launching. As the programmer, you only need to identify parts of your code feeble for parallelization, while the library itself is an abstractions that helps with loop indexing, thread launching, built-in locks, and is portable on different os, making it a nice option for users who don't care about hardware optimations for every different machine. 

# How to Run
```
# for just testing
make test
tests/test -T <test id>
# for just benchmarking
make
src/main -N <numThreads> -T <problem id>
# for testing and benchmarking
# ./run.sh
```
# Description

Follow this [youtube link from Geometry Lab](https://www.youtube.com/watch?v=NDGpsfwAaqo&t=705s&ab_channel=GeometryLab) for a more detailed explanation of a skiplist. Here is a summary of the properties of skiplist. 

| Insertion | Deletion | Search |
| --------- | -------- | ------ |
| log(n)    | log(n)   | log(n) |

\*expected time

The skiplist is able to achieve these marks because it uses layers/levels of linked list (visually) stacked on top of each other for *skipping* sections of a normal linked list (which has linear time search), and thus tries to emulate the methodology behind *binary search*. 

# Coarse-Grain

Coarse-Grain is an adjective that describes something that is granular but still rough in texture. In the context of locking, a coarse-grain lock locks the entire data structure itself. For example, say there are 2 threads, t0 and t1. If t0 and t1 both want to insert an element, and t1 gets the lock first, then t0 will have to wait until t1 unsets the lock before access to any part of the data structure. This is in constrast to fine-grain locking.

Coarse-grain locking is easy to implement since we just place a big ol' lock over the data structure access itself. For few threads and infrequent accesses, coarse-grain can be worth the small downside of slower access times (than fine-grain) for a very easy implementation.

There won't be too much analysis here, since coarse-grain is a trivial adjustment to sequential implementation (just add a lock).