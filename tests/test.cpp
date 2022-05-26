
#include "test.h"
#include "../skiplist.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <memory>


#include "../debug.h"


bool test1(int numThreads, int testID) {

    printf("Test 1[Simple]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert_with_height(11, 1); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->search(11);
    delete slist;
    return v;
}

bool test2(int numThreads, int testID) {

    printf("Test 2[2 insertions at front]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert_with_height(11, 2);
    slist->insert_with_height(5, 1);
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->search(11) && slist->search(5);
    delete slist;
    return v;
}
 
bool test3(int numThreads, int testID) {

    printf("Test 3[2 insertions at back]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert_with_height(5, 1);
    slist->insert_with_height(11, 2);
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->search(11) && slist->search(5);
    delete slist;
    return v;
}

bool test4(int numThreads, int testID) {
    printf("Test 4[3 insertions in the middle]: \n");
    SkipList* slist = new SkipList();
    slist->insert_with_height(2, 1);
    slist->insert_with_height(11, 1);
    slist->insert_with_height(5, 2);
    slist->printList();
    bool v = slist->search(2) && slist->search(5) && slist->search(11);
    delete slist;
    return v;
}

bool test5(int numThreads, int testID) {

    printf("Test 5[insertions insertions randomly (small)]: \n");
    srand(time(NULL));
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 25;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        slist->insert(A[i]);
    }
    slist->printList();
    for (auto i : A) {
        if (!slist->search(A[i])) {
            delete slist;
            return false;
        }
    }
    delete slist;
    return true;
}

bool test6(int numThreads, int testID) {
    printf("Test 6[insertions insertions randomly (big)]: \n");
    srand(time(NULL));
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 10000;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        slist->insert(A[i]);
    }
    // slist->printList();
    for (auto i : A) {
        if (!slist->search(A[i])) {
            delete slist;
            return false;
        }
    }
    delete slist;
    return true;
}

bool test7(int numThreads, int testID) {
    printf("Test 7[Simple]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert_with_height(11, 1); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Removing")
    slist->remove(11); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->empty();
    delete slist;
    return v;
}

bool test8(int numThreads, int testID) {

    printf("Test 8[remove at front]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert_with_height(11, 2);
    slist->insert_with_height(5, 1);
    printdebug("Printing")
    slist->printList();
    printdebug("removing")
    slist->remove(5);
    printdebug("Checking")
    bool v = slist->search(11) && !slist->search(5);
    delete slist;
    return v;
}

bool test9(int numThreads, int testID) {

    printf("Test 9[remove at back]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert_with_height(5, 1);
    slist->insert_with_height(11, 2);
    printdebug("Printing")
    slist->printList();
    printdebug("removing");
    slist->remove(11);
    printdebug("Checking")
    bool v = !slist->search(11) && slist->search(5);
    delete slist;
    return v;
}

bool test10(int numThreads, int testID) {
    printf("Test 10[1 removal in the middle]: \n");
    SkipList* slist = new SkipList();
    slist->insert_with_height(2, 1);
    slist->insert_with_height(11, 1);
    slist->insert_with_height(5, 2);
    slist->printList();
    slist->remove(5);
    slist->printList();
    bool v = slist->search(2) && !slist->search(5) && slist->search(11);
    delete slist;
    return v;
}

bool test11(int numThreads, int testID) {

    printf("Test 11[remove half (small)]: \n");
    srand(time(NULL));
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 25;
    int threshold = 12;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        if (i == threshold) printdebug("All inserted above ^");
        slist->insert(A[i]);
    }
    slist->printList();
    for (int i=0; i<threshold; i++) {
        slist->remove(A[i]);
    }
    slist->printList();
    for (int i=threshold; i<size; i++) {
        slist->remove(A[i]);
    }
    delete slist;
    return true;
}

bool test12(int numThreads, int testID) {

    printf("Test 12[remove half (medium)]: \n");
    srand(time(NULL));
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 10000;
    int threshold = 12;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        if (i == threshold) printdebug("All inserted above ^");
        slist->insert(A[i]);
    }
    for (int i=0; i<size; i++) {
        slist->remove(A[i]);
    }
    delete slist;
    return true;
}
