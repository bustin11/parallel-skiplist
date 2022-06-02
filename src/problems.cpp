
#include "omp.h"
#include "../skiplist.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

// Testing lock functionality
void prob1(int numThreads) {
    srand(time(NULL));
    omp_set_num_threads(numThreads);
    int size = 10000;
    SkipList* slist = new SkipList();

    // insertions
    printf("inserting\n");
    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }
#pragma omp parallel for schedule(static) shared(slist)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
        // slist->printList();
        // printf("========================\n");
    }
        // slist->printList();
        // printf("========================\n");
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
#pragma omp parallel for schedule(static) shared(slist)
    for (int i=0; i<size; i++) {
        // printf("Thread %d: A[%d]=%d\n", omp_get_thread_num(), i, A[i]);
        slist->remove(A[i]);
        // printf("Thread %d: done\n", omp_get_thread_num());
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
            printf(", %d", 2*j+1);
            fflush(stdout);
            SkipList* slist = new SkipList();
            std::vector<key_t> A;
            for (int i=0; i<size; i++) {
                A.push_back(rand()%size);
            }

            // insertions
            double first = omp_get_wtime();
        #pragma omp parallel for schedule(static) shared(slist)
            for (int i=0; i<size; i++) {
                slist->insert(A[i]);
            }
            double second = omp_get_wtime();
            insert_avgTime += second - first;

            printf(", %d", 2*j+2);
            fflush(stdout);
            // deletions 
            first = omp_get_wtime();
        #pragma omp parallel for schedule(static) shared(slist)
            for (int i=0; i<size; i++) {
                slist->remove(A[i]);
            }
            second = omp_get_wtime();
            delete_avgTime += second - first;
            assert(slist->empty());
            delete slist;
        }
        printf("\n[%d insertions (3x avg)]: %f\n", size, (insert_avgTime) / 3);
        printf("[%d deletions] (3x avg): %f\n", size, (delete_avgTime) / 3);
        size += 10000;
    }

}

void prob3(int numThreads) {
    srand(time(NULL));
    omp_set_num_threads(numThreads);
    int size = 10000;
    SkipList* slist = new SkipList();

    // insertions
    printf("Inserting\n");
    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }

#pragma omp parallel for schedule(static) shared(slist)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);


    }



    for (int i=0; i<size; i++) {
        if (!slist->search(A[i])) {
            printf("A[%d]=%d\n", i, A[i]);
            assert(false);
        }
    }

    // deletions 
#pragma omp parallel for schedule(static) shared(slist)
    for (int i=0; i<size; i++) {

        slist->remove(A[i]);

    }

    if (!slist->empty()) {
        slist->printList();
        assert(false);
    }
    delete slist;


}
