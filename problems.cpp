
#include "omp.h"
#include "skiplist.h"

#include <stdlib.h>
#include <time.h>


void prob1(int numThreads) {
    srand(time(NULL));
    SkipList* slist = new SkipList();
    omp_set_num_threads(numThreads);
    int size = 10000;
    double startTime;
    double endTime;
    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand() % size);
        slist->insert(rand() % size);
    }

    // insertions
    startTime = omp_get_wtime();
    #pragma omp parallel for schedule(static) shared(size)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
    }
    endTime = omp_get_wtime();
    printf("[%d insertions]: %f\n", size, endTime-startTime);

    // deletions 
    startTime = omp_get_wtime();
    #pragma omp parallel for schedule(static) shared(size)
    for (int i=0; i<size; i++) {
        slist->remove(A[i]);
    }
    endTime = omp_get_wtime();
    printf("[%d deletions]: %f\n", size, endTime-startTime);

    delete slist;
}
