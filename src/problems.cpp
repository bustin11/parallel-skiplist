
#include "../skiplist.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <fstream>

void write_to_file(int size, float time, std::string desc, std::string fileName) {
    FILE* fp;
    fp = fopen(fileName.c_str(), "a");
    fprintf(fp, "size=%d, %s=%f\n", size, desc.c_str(), time);
    fclose(fp);
}

void prob_size(int numThreads, int size, std::string fileName) {

    srand(time(NULL));
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
        clock_t first = clock();
        for (int i=0; i<size; i++) {
            slist->insert(A[i]);
        }
        clock_t second = clock();
        insert_avgTime += second - first;

        printf(", %d", 2*j+2);
        fflush(stdout);
        // deletions 
        first = clock();
        #pragma omp parallel for schedule(dynamic)
        for (int i=0; i<size; i++) {
            slist->remove(A[i]);
        }
        second = clock();
        delete_avgTime += second - first;
        // assert(slist->empty());
    }
    insert_avgTime /= (3 * CLOCKS_PER_SEC);
    delete_avgTime /= (3 * CLOCKS_PER_SEC);
    printf("\n[%d insertions (3x avg)]: %f\n", size, insert_avgTime);
    printf("[%d deletions] (3x avg): %f\n", size, delete_avgTime);
    write_to_file(size, insert_avgTime, "insert time", fileName);
    write_to_file(size, delete_avgTime, "delete time", fileName);
}


void prob_size_mixed(int numThreads, int size, float p, std::string fileName) {

    srand(time(NULL));
    double avgTime = 0;

    auto biased_coin_flip = [](float p){
        return rand()/RAND_MAX < p;
    };

    printf("size=%d, ", size);
    printf("p=%.2f", p);
    fflush(stdout);
    for (int j=0; j<3; j++) {
        printf(", %d", j+1);
        fflush(stdout);
        SkipList* slist = new SkipList();
        std::vector<key_t> A;
        for (int i=0; i<size; i++) {
            A.push_back(rand()%(size/10));
        }

        clock_t first = clock();
        #pragma omp parallel for schedule(dynamic)
        for (int i=0; i<size; i++) {
            if (biased_coin_flip(p)) slist->insert(A[i]);
            else slist->remove(A[i]);
        }
        clock_t second = clock();
        avgTime += second - first;

    }
    avgTime /= (3 * CLOCKS_PER_SEC);
    printf("\n[%d ins/del (3x avg)]: %f\n", size, (avgTime) / 3);
    write_to_file(size, avgTime, "ins/del time", fileName);
}