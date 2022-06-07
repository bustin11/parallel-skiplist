
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <atomic>
#include <memory>

#include "omp.h"
#include "skiplist.h"
#include "helpers/debug.h"
#include "helpers/helpers.h"


SkipList::SkipList() {
    this->head = 
        new Node(INT32_MIN, this->MAX_LEVEL); // starting dummy node
}

int SkipList::search_prev (key_t key, 
                            std::vector<Node*>& preds) const {

    Node* prev = this->head;
    int heightOfInsertion = static_cast<int>(preds.size());
    int keyFound = -1;

    for (int i=heightOfInsertion-1; i>=0; i--) {

        // curr is either the node with the key value, or is guarenteed to be
        // less than the key value
        Node* curr = prev->next[i];

        while (curr && key > curr->key) { // hottest instruction
            prev = curr;
            curr = prev->next[i];
        }
        // finds the top most insertion point
        if (keyFound < 0 && curr && key == curr->key) {
            keyFound = i;
        }
        if (i < heightOfInsertion) {
            preds[i] = prev;
        }
    }

    return keyFound;
}

bool SkipList::search (key_t key) const {

    std::vector<Node*> A(this->MAX_LEVEL); // not use :(
    int found = this->search_prev(key, A);
    return found >= 0 
            && A[found]->next[found] 
            && A[found]->next[found]->key == key;

}


bool SkipList::insert (key_t key) {

    int insertedHeight = randomHeight(this->MAX_LEVEL);
    std::vector<Node*> preds(insertedHeight); // index 0: lowest 


    omp_set_lock(&this->lock);
    int found = search_prev(key, preds);
    if (found >= 0) {
        omp_unset_lock(&this->lock);
        return false;
    }

    Node* newNode = 
        new Node(key, insertedHeight);

    // connect
    for (int i=0; i<insertedHeight; i++) {
        newNode->next[i] = preds[i]->next[i];
        preds[i]->next[i] = newNode;
    }
    omp_unset_lock(&this->lock);

    return true;

}

bool SkipList::remove (key_t key) {

    std::vector<Node*> preds(this->MAX_LEVEL); // index 0: lowest 


    omp_set_lock(&this->lock);
    int found = search_prev(key, preds);
    if (found < 0) {
        omp_unset_lock(&this->lock);
        return false;
    }

    // disconnect
    Node* remember = preds[0]->next[0];
    for (int i=0; i<=found; i++) {
        preds[i]->next[i] = remember->next[i];
    }
    delete remember;
    omp_unset_lock(&this->lock);

    return true;

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