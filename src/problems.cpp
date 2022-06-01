
#include "omp.h"
#include "../skiplist.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

// Testing lock functionality
void prob1(int numThreads) {
    srand(11);
    omp_set_num_threads(numThreads);
    int size = 10;
    SkipList* slist = new SkipList();

    // insertions
    printf("inserting\n");
    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }
    #pragma parallel omp for schedule(static) shared(size)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
        // slist->printList();
        // printf("========================\n");
    }
    // correctness
    for (int i=0; i<size; i++) {
        if (!slist->search(A[i])) {
            printf("A[%d]=%d\n", i, A[i]);
            assert(false);
        }
    }
    // slist->printList();
    // deletions 
    printf("deleting\n");
    #pragma omp parallel for schedule(static) shared(size)
    for (int i=0; i<size; i++) {
        // printf("A[%d]=%d\n", i, A[i]);
        slist->remove(A[i]);
        // printf("done\n");
    }
    if (!slist->empty()) {
        slist->printList();
        assert(false);
    }
    delete slist;

}

void prob2(int numThreads) {
    srand(time(NULL));
    omp_set_num_threads(numThreads);
    int size = 10000;
    double insert_avgTime = 0;
    double delete_avgTime = 0;

    for (int k=0; k<3; k++) {
        printf("size=%d", size);
        fflush(stdout);
        for (int j=0; j<3; j++) {
            printf(", %d", j+1);
            fflush(stdout);
            SkipList* slist = new SkipList();
            std::vector<key_t> A;
            for (int i=0; i<size; i++) {
                A.push_back(rand() % size);
            }

            // insertions
            printf("inserting\n");
            double first = omp_get_wtime();
            #pragma omp parallel for schedule(static) shared(size)
            for (int i=0; i<size; i++) {
                slist->insert(A[i]);
            }
            double second = omp_get_wtime();
            insert_avgTime += second - first;

            // deletions 
            printf("deleting\n");
            first = omp_get_wtime();
            #pragma omp parallel for schedule(static) shared(size)
            for (int i=0; i<size; i++) {
                slist->remove(A[i]);
            }
            second = omp_get_wtime();
            delete_avgTime += second - first;
            // if (!slist->empty());
            delete slist;
        }
        printf("\n[%d insertions (3x avg)]: %f\n", size, (insert_avgTime) / 3);
        printf("[%d deletions] (3x avg): %f\n", size, (delete_avgTime) / 3);
        size += 10000;
    }

}
