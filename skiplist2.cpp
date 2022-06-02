
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <atomic>

#include "skiplist.h"
#include "helpers/debug.h"
#include "helpers/helpers.h"


SkipList::SkipList() {
    this->head = new Node(INT32_MIN, this->MAX_LEVEL); // starting dummy node
}

SkipList::~SkipList() {

    std::atomic<Node*> curr;
    curr.store(this->head.load()->next[0]);
    delete this->head;
    while (curr) {
        std::atomic<Node*> right;
        right.store(curr.load()->next[0]);
        omp_destroy_lock(&curr.load()->lock);
        delete curr;
        curr.store(right);
    }
}

int SkipList::search_prev (key_t key, 
                            std::vector<std::atomic<Node*>>& preds,
                            std::vector<std::atomic<Node*>>& succs) const {

    int threadID = omp_get_thread_num();
    std::atomic<Node*> prev = this->head.load();
    // int numOfLevels = static_cast<int>(this->head->next.size());
    int heightOfInsertion = static_cast<int>(preds.size());
    int keyFound = -1;

    for (int i=heightOfInsertion-1; i>=0; i--) {
        std::atomic<Node*> curr;
        curr.store(prev.load()->next[i]);
        if (prev.load()->height == 5)
            break;

        while (curr && key > curr.load()->key) {
            prev.store(curr);
            curr.store(prev.load()->next[i]);
        }
        if (keyFound < 0 && curr && key == curr.load()->key) {
            keyFound = i;
        }
        if (i < heightOfInsertion) {
            preds[i].store(prev);
            succs[i].store(curr);
        }
    }

    return keyFound;
}

// TODO: make this thread safe
bool SkipList::search (key_t key) const {

    std::vector<std::atomic<Node*>> A(this->MAX_LEVEL); // not use :(
    std::vector<std::atomic<Node*>> B(this->MAX_LEVEL);
    int found = this->search_prev(key, A, B);
    return found >= 0 && B[0] 
        && B[0].load()->key == key && B[0].load()->is_fully_linked()
        && !B[0].load()->is_marked();

}

bool SkipList::insert (key_t key) {

    int insertedHeight = randomHeight(this->MAX_LEVEL);
    std::vector<std::atomic<Node*>> preds(insertedHeight); // index 0: lowest 
    std::vector<std::atomic<Node*>> succs(insertedHeight); // index 0: lowest 

    while (true) {

        int found = search_prev(key, preds, succs);
        if (found >= 0) {
            if (succs[found] && !succs[found].load()->is_marked()) {
                while (!succs[found].load()->is_fully_linked()) {}
                return false;
            }
            continue;
        }

        int highestLocked = -1;
        bool abort = false;
        for (int i=0; i<insertedHeight; i++) {
            std::atomic<Node*> pred = preds[i].load();
            highestLocked = i;
            if (i == 0 || pred != preds[i-1]) {
                if (pred.load()->is_marked() 
                || !omp_test_lock(&pred.load()->lock)) {
                    abort = true;
                    break;
                }
            }
        }
        if (abort) {
            for (int j=0; j<highestLocked; j++) {
                omp_unset_lock(&preds[j].load()->lock);
            }
            continue;
        }

        std::atomic<Node*> newNode = new Node(key, insertedHeight);

        for (int i=0; i<insertedHeight; i++) {
            std::atomic<Node*> succ;
            succ.store(preds[i].load()->next[i]);
            preds[i].load()->next[i].store(newNode);
            newNode.load()->next[i].store(succ);
        }

        newNode.load()->set_fully_linked();

        for (int i=0; i<insertedHeight; i++) {
            omp_unset_lock(&preds[i].load()->lock);
        }

        return true;

    }

    // SHOULD NOT REACH HERE!
    return false;
    
}


bool SkipList::remove (key_t key) {

    int threadID = omp_get_thread_num();
    std::vector<std::atomic<Node*>> preds(this->MAX_LEVEL); // index 0: lowest 
    std::vector<std::atomic<Node*>> succs(this->MAX_LEVEL); // index 0: lowest 
    std::atomic<Node*> victim;
    bool marked = false;

    while (true) {

        int found = search_prev(key, preds, succs);
        if (found >= 0) 
            victim.store(succs[found]);
        if (marked ||
            (found >= 0 &&
            found+1 == victim.load()->height &&
            victim.load()->is_fully_linked() &&
            !victim.load()->is_marked())) { // being removed

            if (!marked) {
                if (victim.load()->is_marked()) {
                    return false;
                }
                if (!omp_test_lock(&victim.load()->lock)) {
                    continue;
                }
                marked = true;
                victim.load()->set_marked();
            }
            int height = victim.load()->height;

            #if _OMP_H==1
                int highestLocked = -1;
                bool abort = false;
                for (int i=0; i<height; i++) {
                    std::atomic<Node*> pred = preds[i].load();
                    highestLocked = i;
                    bool v = !omp_test_lock(&pred.load()->lock);
                    if (( v
                    && (i == 0 || pred != preds[i-1]))
                    || pred.load()->is_marked() 
                    || pred.load()->next[i] != victim) {
                        abort = true;
                        break;
                    }
                }
                if (abort) {
                    for (int i=highestLocked; i>=0; i--){
                        omp_unset_lock(&preds[i].load()->lock);
                    }
                    continue;
                }
            #endif

            for (int i=height-1; i>=0; i--) {
                preds[i].load()->next[i].store(victim.load()->next[i]);
            }
            delete victim;

            for (int i=highestLocked; i>=0; i--){
                omp_unset_lock(&preds[i].load()->lock);
            }

            return true;
        } else {
            return false;
        }

    }

    // SHOULD NOT REACH HERE!
    return false;

}


void SkipList::printList () const {

    // find the offsets for printing based on level 0
    std::vector<std::pair<int, std::atomic<Node*>>> offsets;
    std::atomic<Node*> curr;
    curr.store(this->head.load()->next[0]);
    int offset = 0;
    while (curr) {
        offsets.push_back(std::make_pair(offset, curr));
        std::string itemstr = curr.load()->toStr();
        offset += static_cast<int>(itemstr.length() + 1);
        curr.store(curr.load()->next[0]);
    }

    int numOffsets = static_cast<int>(offsets.size());
    int numRight = static_cast<int>(this->head.load()->next.size());
    while (!this->head.load()->next[--numRight]) {}

    for (int i=numRight; i>=0; i--) { // horizontal
        curr.store(this->head.load()->next[i]);
        int p = 0;
        int prevLength = 0;
        for (int j=0; j<numOffsets; j++) { // vertical
            if (offsets[j].second == curr) {
                // find difference, subtract for spaces, subtract prevLength
                // because that doesn't count towards the dashes
                int numDashes = offsets[j].first-offsets[p].first-prevLength;
                if (numDashes > 2) {
                    printf(" ");
                    for (int k=0; k<numDashes-2; k++) printf("-");
                    printf(" ");
                } else {
                    for (int k=0; k<numDashes; k++) printf(" ");
                }
                
                std::string itemstr = curr.load()->toStr();
                printf("%s", itemstr.c_str());
                curr.store(curr.load()->next[i]);
                p = j;
                prevLength = static_cast<int>(itemstr.length());
            } 
        }
        printNewLine();
    }
}

bool SkipList::empty () const {
    return this->head.load()->next[0] == nullptr;
}