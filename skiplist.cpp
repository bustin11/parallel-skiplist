
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <atomic>
#include <memory>

#include "skiplist.h"
#include "helpers/debug.h"
#include "helpers/helpers.h"


SkipList::SkipList() {
    this->head = 
        std::make_shared<Node>(INT32_MIN, this->MAX_LEVEL); // starting dummy node
}

SkipList::~SkipList() {

    // Node* curr = this->head->next[0];
    // delete this->head;
    // while (curr) {
    //     Node* right = curr->next[0];
    //     omp_destroy_lock(&curr->lock);
    //     delete curr;
    //     curr = right;
    // }

    // for (auto node : deleteList) {
    //     delete node;
    // }

}

int SkipList::search_prev (key_t key, 
                            std::vector<std::shared_ptr<Node>>& preds,
                            std::vector<std::shared_ptr<Node>>& succs) const {

    // int threadID = omp_get_thread_num();
    std::shared_ptr<Node> prev = this->head;
    // int numOfLevels = static_cast<int>(this->head->next.size());
    int heightOfInsertion = static_cast<int>(preds.size());
    int keyFound = -1;

    for (int i=heightOfInsertion-1; i>=0; i--) {

        std::shared_ptr<Node> curr = prev->next[i];

        while (curr && key > curr->key) {
                
            prev = curr;
            curr = prev->next[i];

        }
        if (keyFound < 0 && curr && key == curr->key) {
            keyFound = i;
        }
        if (i < heightOfInsertion) {
            preds[i] = prev;
            succs[i] = curr;
        }
    }

    return keyFound;
}

// TODO: make this thread safe
bool SkipList::search (key_t key) const {

    std::vector<std::shared_ptr<Node>> A(this->MAX_LEVEL); // not use :(
    std::vector<std::shared_ptr<Node>> B(this->MAX_LEVEL);
    int found = this->search_prev(key, A, B);
    return found >= 0 && B[0] 
        && B[0]->key == key && B[0]->is_fully_linked()
        && (!B[0]->is_marked());

}

bool SkipList::insert (key_t key) {

    int insertedHeight = randomHeight(this->MAX_LEVEL);
    std::vector<std::shared_ptr<Node>> preds(insertedHeight); // index 0: lowest 
    std::vector<std::shared_ptr<Node>> succs(insertedHeight); // index 0: lowest 

    while (true) {

        int found = search_prev(key, preds, succs);
        if (found >= 0) {
            if (succs[found] && !succs[found]->is_marked()) {
                while (!succs[found]->is_fully_linked()) {}
                return false;
            }
            continue;
        }

        bool abort = false;
        bool v;
        bool a;
        bool b;
        bool c;
        std::vector<std::shared_ptr<Node>> locksAquired;
        for (int i=0; i<insertedHeight; i++) {
            std::shared_ptr<Node> pred = preds[i];
            // if (i == 0 || pred != preds[i-1]) {
            v = pred->is_marked();
            a = !omp_test_lock(&pred->lock);
            if (!a) {
                locksAquired.push_back(pred);
            }
            b = (i == 0 || pred != preds[i-1]);
            c = succs[i] != pred->next[i];
            // if (pred->is_marked() 
            // || !omp_test_lock(&pred->lock)) {
            if ((a && b) || v || c) {
                abort = true;
                break;
            }
            // }
        }
        if (abort) {
            for (auto& pred : locksAquired) {
                omp_unset_lock(&pred->lock);
            }
            continue;
        }

        std::shared_ptr<Node> newNode = 
            std::make_shared<Node>(key, insertedHeight);

        for (int i=0; i<insertedHeight; i++) {
            newNode->next[i] = preds[i]->next[i];
            preds[i]->next[i] = newNode;
        }

        newNode->set_fully_linked();

        for (int i=0; i<insertedHeight; i++) {
            if (i == 0 || preds[i] != preds[i-1])
                omp_unset_lock(&preds[i]->lock);
        }

        return true;

    }

    // SHOULD NOT REACH HERE!
    return false;
    
}


bool SkipList::remove (key_t key) {

    // int threadID = omp_get_thread_num();
    std::vector<std::shared_ptr<Node>> preds(this->MAX_LEVEL); // index 0: lowest 
    std::vector<std::shared_ptr<Node>> succs(this->MAX_LEVEL); // index 0: lowest 
    bool marked = false;

    while (true) {

        std::shared_ptr<Node> victim;
        int found = search_prev(key, preds, succs);
        if (found >= 0) 
            victim = succs[found];
        if (marked ||
            (found >= 0 &&
            found+1 == victim->height &&
            victim->is_fully_linked() &&
            !victim->is_marked())) { // being removed

            if (!marked) {
                if (victim->is_marked()) {
                    return false;
                }
                if (!omp_test_lock(&victim->lock)) {
                    continue;
                }
                marked = true;
                victim->set_marked();
            }
            int height = victim->height;

            #if _OMP_H==1
                int highestLocked = -1;
                bool abort = false;
                for (int i=0; i<height; i++) {
                    std::shared_ptr<Node> pred = preds[i];
                    highestLocked = i;
                    bool v = !omp_test_lock(&pred->lock);
                    // if (( v
                    // && (i == 0 || pred != preds[i-1]))
                    // || pred->is_marked() 
                    // || pred->next[i] != victim) {
                    bool a (i == 0 || pred != preds[i-1]);
                    bool b = pred->is_marked();
                    bool c = pred->next[i] != victim;
                    if ((v && a) || b || c) {
                        abort = true;
                        break;
                    }
                }
                if (abort) {
                    for (int i=0; i<highestLocked; i++){
                        if (i == 0 || preds[i] != preds[i-1])
                            omp_unset_lock(&preds[i]->lock);
                    }
                    continue;
                }
            #endif

            for (int i=0; i<height; i++){
                preds[i]->next[i] = victim->next[i];
            }

            for (int i=0; i<height; i++){
                if (i == 0 || preds[i] != preds[i-1])
                    omp_unset_lock(&preds[i]->lock);
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
    std::vector<std::pair<int, std::shared_ptr<Node>>> offsets;
    std::shared_ptr<Node> curr = this->head->next[0];
    int offset = 0;
    while (curr) {
        offsets.push_back(std::make_pair(offset, curr));
        std::string itemstr = curr->toStr();
        offset += static_cast<int>(itemstr.length() + 1);
        curr = curr->next[0];
    }

    int numOffsets = static_cast<int>(offsets.size());
    int numRight = static_cast<int>(this->head->next.size());
    while (!this->head->next[--numRight]) {}

    for (int i=numRight; i>=0; i--) { // horizontal
        curr = this->head->next[i];
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
                
                std::string itemstr = curr->toStr();
                printf("%s", itemstr.c_str());
                curr = curr->next[i];
                p = j;
                prevLength = static_cast<int>(itemstr.length());
            } 
        }
        printNewLine();
    }
    
}

bool SkipList::empty () const {
    return this->head->next[0] == nullptr;
}