
#include "omp.h"
#include "../skiplist.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

void prob_size(int numThreads, int size) {

    srand(time(NULL));
    omp_set_num_threads(numThreads);
    double insert_avgTime = 0;
    double delete_avgTime = 0;

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
    #pragma omp parallel for schedule(dynamic) shared(slist)
        for (int i=0; i<size; i++) {
            slist->insert(A[i]);
        }
        double second = omp_get_wtime();
        insert_avgTime += second - first;

        printf(", %d", 2*j+2);
        fflush(stdout);
        // deletions 
        first = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic) shared(slist)
        for (int i=0; i<size; i++) {
            slist->remove(A[i]);
        }
        second = omp_get_wtime();
        delete_avgTime += second - first;
        // assert(slist->empty());
    }
    printf("\n[%d insertions (3x avg)]: %f\n", size, (insert_avgTime) / 3);
    printf("[%d deletions] (3x avg): %f\n", size, (delete_avgTime) / 3);

}


void prob_size_mixed(int numThreads, int size, float p) {

    srand(time(NULL));
    omp_set_num_threads(numThreads);
    double avgTime = 0;

    auto biased_coin_flip = [](float p){
        return rand()/RAND_MAX < p;
    };

    printf("size=%d, ", size);
    printf("p=%.2f, ", p);
    fflush(stdout);
    for (int j=0; j<3; j++) {
        printf(", %d", j+1);
        fflush(stdout);
        SkipList* slist = new SkipList();
        std::vector<key_t> A;
        for (int i=0; i<size; i++) {
            A.push_back(rand()%(size/10));
        }

        double first = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic) shared(slist)
        for (int i=0; i<size; i++) {
            if (biased_coin_flip(p)) slist->insert(A[i]);
            else slist->remove(A[i]);
        }
        double second = omp_get_wtime();
        avgTime += second - first;

    }
    printf("\n[%d ins/del (3x avg)]: %f\n", size, (avgTime) / 3);

}