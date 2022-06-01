
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>
#include <functional>

#include "skiplist.h"
#include "helpers/debug.h"
#include "helpers/helpers.h"


SkipList::SkipList() {
    this->head = new Node(INT32_MIN, this->MAX_LEVEL); // starting dummy node
    this->head->next[0] = nullptr;
}

SkipList::~SkipList() {

    Node* curr = this->head->next[0];
    delete this->head;
    while (curr) {
        Node* right = curr->next[0];
        omp_destroy_lock(&curr->lock);
        delete curr;
        curr = right;
    }
}

int SkipList::search_prev (key_t key, 
                            std::vector<Node*>& preds,
                            std::vector<Node*>& succs) const {

    Node* prev = this->head;
    int numOfLevels = static_cast<int>(this->head->next.size());
    int heightOfInsertion = static_cast<int>(preds.size());
    int keyFound = -1;

    for (int i=numOfLevels-1; i>=0; i--) {
        Node* curr = prev->next[i];
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

    std::vector<Node*> A(this->MAX_LEVEL); // not use :(
    std::vector<Node*> B(this->MAX_LEVEL);
    int found = this->search_prev(key, A, B);
    return found >= 0 && B[0] 
        && B[0]->key == key && B[0]->is_fully_linked()
        && !B[0]->is_marked();

}

bool SkipList::insert (key_t key) {

    int insertedHeight = randomHeight(this->MAX_LEVEL);
    std::vector<Node*> preds(insertedHeight); // index 0: lowest 
    std::vector<Node*> succs(insertedHeight); // index 0: lowest 

    while (true) {

        int found = search_prev(key, preds, succs);
        if (found >= 0) {
            Node* nodeFound = succs[found];
            if (nodeFound && !nodeFound->is_marked()) {
                while (!nodeFound->is_fully_linked()) {}
                return false;
            } else {
                continue;
            }
        }

        #if _OMP_H==1
            int highestLocked = -1;
            bool valid = true;
            for (int i=0; valid && i<insertedHeight; i++) {
                Node* pred = preds[i];
                Node* succ = succs[i];
                highestLocked = i;
                valid = !pred->is_marked() 
                        && ((succ 
                        && !succ->is_marked())
                        || !succ)
                        && pred->next[i] == succ
                        && ((i > 0 && pred == preds[i-1])
                            || omp_test_lock(&pred->lock));
            }
            if (!valid) {
                for (int i=0; i<highestLocked; i++) {
                    omp_unset_lock(&preds[i]->lock);
                }
            }
        #endif

        Node* newNode = new Node(key, insertedHeight);

        for (int i=0; i<insertedHeight; i++) {
            preds[i]->next[i] = newNode;
            newNode->next[i] = succs[i];
        }
        newNode->set_fully_linked();

        for (int i=0; i<insertedHeight; i++) {
            omp_unset_lock(&preds[i]->lock);
        }

        return true;

    }

    // SHOULD NOT REACH HERE!
    return false;
    
}


bool SkipList::remove (key_t key) {

    std::vector<Node*> preds(this->MAX_LEVEL); // index 0: lowest 
    std::vector<Node*> succs(this->MAX_LEVEL); // index 0: lowest 
    Node* victim;
    bool marked = false;

    while (true) {

        int found = search_prev(key, preds, succs);
        if (found >= 0) victim = succs[found];
        if (marked ||
            (found >= 0 &&
            found+1 == victim->height &&
            victim->is_fully_linked() &&
            !victim->is_marked())) { // being removed

            int height = victim->height;
            if (!marked) {
                if (!omp_test_lock(&victim->lock) || victim->is_marked()) {
                    omp_unset_lock(&victim->lock);
                    return false;
                }
                marked = true;
                victim->set_marked();
            }

            #if _OMP_H==1
                int highestLocked = -1;
                bool valid = true;
                for (int i=0; valid && i<height; i++) {
                    Node* pred = preds[i];
                    highestLocked = i;
                    valid = 
                        !pred->is_marked() 
                        && pred->next[i] == victim
                        && ((i > 0 && pred == preds[i])
                        || omp_test_lock(&pred->lock));
                }
                if (!valid) {
                    for (int i=0; i<highestLocked; i++) {
                        omp_unset_lock(&preds[i]->lock);
                    }
                }
            #endif


            victim->set_marked(); 

            for (int i=height-1; i>=0; i--) {
                preds[i]->next[i] = succs[i]->next[i];
            }
            omp_unset_lock(&victim->lock);
            delete victim;

            for (int i=0; i<height; i++) {
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
    std::vector<std::pair<int, Node*>> offsets;
    Node* curr = this->head->next[0];
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