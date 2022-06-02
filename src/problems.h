
#ifndef _PROBLEMS_H
#define _PROBLEMS_H

/*
@param numThreads: Number of threads to launch
@param size: Number of elements to insert 

@return void

Runs benchmarking performances. Code is not commented but
easy to read regardless.
*/
void prob_size(int numThreads, int size);
void prob_size_mixed(int numThreads, int size, float p);

#endif