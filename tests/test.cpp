
#include "test.h"
#include "../helpers/debug.h"
#include "../skiplist.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <memory>


#include "../helpers/debug.h"


bool test1() {

    printf("Test 1[Simple]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(11); // height == 1
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->search(11);
    delete slist;
    return v;
}

bool test2() {

    printf("Test 2[2 insertions at front]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(11);
    if (DEBUG) slist->printList();
    slist->insert(5);
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->search(11) && slist->search(5);
    delete slist;
    return v;
}
 
bool test3() {

    printf("Test 3[2 insertions at back]: \n");

    SkipList* slist = new SkipList();
    printdebug("Insertion")
    slist->insert(5);
    if (DEBUG) slist->printList();
    slist->insert(11);
    printdebug("Printing")
    slist->printList();
    printdebug("Checking")
    bool v = slist->search(11) && slist->search(5);
    delete slist;
    return v;
}

bool test4() {
    printf("Test 4[3 insertions in the middle]: \n");
    SkipList* slist = new SkipList();
    slist->insert(2);
    if (DEBUG) slist->printList();
    slist->insert(11);
    if (DEBUG) slist->printList();
    slist->insert(5);
    slist->printList();
    bool v = slist->search(2) && slist->search(5) && slist->search(11);
    delete slist;
    return v;
}

bool test5() {

    printf("Test 5[insertions insertions randomly (small)]: \n");
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 25;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        slist->insert(A[i]);
        if (DEBUG) slist->printList();
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

bool test6() {
    printf("Test 6[insertions insertions randomly (big)]: \n");
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
    bool v = slist->empty();
    delete slist;
    return v;
}

bool test8() {

    printf("Test 8[remove at front]: \n");

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
    bool v = slist->search(11) && !slist->search(5);
    delete slist;
    return v;
}

bool test9() {

    printf("Test 9[remove at back]: \n");

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
    bool v = !slist->search(11) && slist->search(5);
    delete slist;
    return v;
}

bool test10() {
    printf("Test 10[1 removal in the middle]: \n");
    SkipList* slist = new SkipList();
    slist->insert(2);
    slist->insert(11);
    slist->insert(5);
    printdebug("Printing");
    slist->printList();
    printdebug("removing");
    slist->remove(5);
    if (DEBUG) slist->printList();
    bool v = slist->search(2) && !slist->search(5) && slist->search(11);
    delete slist;
    return v;
}

bool test11() {

    printf("Test 11[remove half (small)]: \n");
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 25;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        slist->insert(A[i]);
    }
    slist->printList();
    for (int i=0; i<size; i++) {
        slist->remove(A[i]);
        if (DEBUG) slist->printList();
    }
    bool v = slist->empty();
    delete slist;
    return v;
}

bool test12() {

    printf("Test 12[remove (big)]: \n");
    SkipList* slist = new SkipList();
    std::vector<key_t> A;
    int size = 10000;
    for (int i=0; i<size; i++) {
        A.push_back(rand()%size);
        slist->insert(A[i]);
    }
    for (int i=0; i<size; i++) {
        slist->remove(A[i]);
    }
    bool v = slist->empty();
    delete slist;
    return v;
}
