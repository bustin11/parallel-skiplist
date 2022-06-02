
#include "test.h"
#include "../helpers/debug.h"
#include "../skiplist.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "omp.h"


bool test1() {

    printf("Test 1[Simple]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(11); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool valid = slist->search(11);
    return valid;
}

bool test2() {

    printf("Test 2[1 insertions at front]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(11);
    if (DEBUG) slist->printList();
    slist->insert(5);
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool valid = slist->search(11) && slist->search(5);
    return valid;
}
 
bool test3() {

    printf("Test 3[1 insertions at back]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(5);
    if (DEBUG) slist->printList();
    slist->insert(11);
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool valid = slist->search(11) && slist->search(5);
    return valid;
}

bool test4() {
    printf("Test 4[1 insertions in the middle]: \n");
    SkipList* slist = new SkipList();
    slist->insert(2);
    if (DEBUG) slist->printList();
    slist->insert(11);
    if (DEBUG) slist->printList();
    slist->insert(5);
    slist->printList();
    bool valid = slist->search(2) && slist->search(5) && slist->search(11);
    return valid;
}

bool test5(int numThreads, int seed) {

    int size = 25;
    printf("Test 5[%d insertions insertions]: \n", size);

    (seed < 0) ? srand(time(NULL)) : srand(seed);
    omp_set_num_threads(numThreads);

    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }

    SkipList* slist = new SkipList();
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
        if (DEBUG) slist->printList();
    }
    slist->printList();

    bool invalid = false;
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (auto i : A) {
        if (!slist->search(A[i])) {
            invalid = true;
        }
    }
    return !invalid;
}

bool test6(int numThreads, int seed) {

    int size = 10000;
    printf("Test 6[%d insertions insertions randomly]: \n", size);

    (seed < 0) ? srand(time(NULL)) : srand(seed);
    omp_set_num_threads(numThreads);
    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }

    SkipList* slist = new SkipList();
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
    }
    // slist->printList();

    bool invalid = false;
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (auto i : A) {
        if (!slist->search(A[i])) {
            invalid = true;
        }
    }
    return !invalid;
}

bool test7() {
    printf("Test 7[Simple]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(11); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Removing")
    slist->remove(11); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool valid = slist->empty();
    return valid;
}

bool test8() {

    printf("Test 8[1 deletion at front]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(11);
    slist->insert(5);
    printdebug("Printing")
    slist->printList();
    printdebug("removing")
    slist->remove(5);
    if (DEBUG) slist->printList();
    printdebug("Checking")
    bool valid = slist->search(11) && !slist->search(5);
    return valid;
}

bool test9() {

    printf("Test 9[1 deletion at back]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(5);
    slist->insert(11);
    printdebug("Printing")
    slist->printList();
    printdebug("removing");
    slist->remove(11);
    if (DEBUG) slist->printList();
    printdebug("Checking")
    bool valid = !slist->search(11) && slist->search(5);
    return valid;
}

bool test10() {

    printf("Test 10[1 deletion in the middle]: \n");
    SkipList* slist = new SkipList();
    slist->insert(2);
    slist->insert(11);
    slist->insert(5);
    printdebug("Printing");
    slist->printList();
    printdebug("removing");
    slist->remove(5);
    if (DEBUG) slist->printList();
    bool valid = slist->search(2) && !slist->search(5) && slist->search(11);
    return valid;
}

bool test11(int numThreads, int seed) {

    int size = 25;
    printf("Test 11[%d deletions]: \n", size);

    (seed < 0) ? srand(time(NULL)) : srand(seed);
    omp_set_num_threads(numThreads);

    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }

    SkipList* slist = new SkipList();
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
    }
    slist->printList();
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (int i=0; i<size; i++) {
        slist->remove(A[i]);
        if (DEBUG) slist->printList();
    }
    bool valid = slist->empty();
    return valid;
}

bool test12(int numThreads, int seed) {

    int size = 10000;
    printf("Test 12[%d random deletions]: \n", size);

    (seed < 0) ? srand(time(NULL)) : srand(seed);
    omp_set_num_threads(numThreads);

    std::vector<key_t> A;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
    }

    SkipList* slist = new SkipList();
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (int i=0; i<size; i++) {
        slist->insert(A[i]);
    }
    #pragma omp parallel for schedule(dynamic) shared(slist)
    for (int i=0; i<size; i++) {
        slist->remove(A[i]);
    }
    bool valid = slist->empty();
    return valid;
}
