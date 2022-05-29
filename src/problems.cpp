
#include "omp.h"
#include "../skiplist.h"

#include <stdlib.h>
#include <time.h>


void prob1(int numThreads) {
    srand(time(NULL));
    omp_set_num_threads(numThreads);
    int size = 10000;
    double insert_avgTime = 0;
    double delete_avgTime = 0;

    for (int k=0; k<3; k++) {
        for (int j=0; j<3; j++) {
            printf("%d\n", j+1);
            SkipList* slist = new SkipList();
            std::vector<key_t> A;
            for (int i=0; i<size; i++) {
                A.push_back(rand() % size);
                slist->insert(rand() % size);
            }

            // insertions
            double first = omp_get_wtime();
            #pragma omp parallel for schedule(static) shared(size)
            for (int i=0; i<size; i++) {
                slist->insert(A[i]);
            }
            double second = omp_get_wtime();
            insert_avgTime += second - first;

            // deletions 
            first = 0;
            second = 0;
            first = omp_get_wtime();
            #pragma omp parallel for schedule(static) shared(size)
            for (int i=0; i<size; i++) {
                slist->remove(A[i]);
            }
            second = omp_get_wtime();
            delete_avgTime += second - first;
            delete slist;
        }
        printf("[%d insertions (3x avg)]: %f\n", size, (insert_avgTime) / 3);
        printf("[%d deletions] (3x avg): %f\n", size, (delete_avgTime) / 3);
        size += 10000;
    }

}
