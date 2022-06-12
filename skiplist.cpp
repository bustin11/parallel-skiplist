
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

extern std::hash<key_t> myhash;

template<typename T>
SkipList<T>::SkipList() {
    this->head = new Node(INT32_MIN, MAX_LEVEL);
    this->tail = new Node(INT32_MAX, MAX_LEVEL);
    for (int i=0; i<MAX_LEVEL; i++) {
        this->head->next[i].set(
            tail,
            false
        );
    }
}

template<typename T>
SkipList<T>::~SkipList() {
    Node* prev = head;
    while (prev) {
        Node* curr = prev->next[0].get_reference();
        delete prev;
        prev = curr;
    }
}

template<typename T>
bool SkipList<T>::search_prev (T val, 
                            std::vector<Node*>& preds,
                            std::vector<Node*>& succs) {

    key_t key = myhash(val); 
    int heightOfInsertion = static_cast<int>(preds.size());
    Node* prev = nullptr;
    Node* curr = nullptr;
    Node* succ = nullptr;

    retry:
    while (true) {
        prev = this->head;
        for (int i=heightOfInsertion-1; i>=0; i--) {
            curr = prev->next[i].get_reference();
            succ = nullptr;
            while (true) {
                bool mark;
                succ = curr->next[i].get(mark);
                // continue snipping, because multiple marked nodes
                // can be present in a row
                while (mark) {
                    bool snip = prev->next[i].CAS(curr, succ, false, false);
                    if (!snip) goto retry;
                    curr = prev->next[i].get_reference();
                    succ = curr->next[i].get(mark);
                }
                // Can we keep going?
                if (curr->key < key) {
                    prev = curr;
                    curr = succ;
                } else {
                    break;
                }
            }
            preds[i] = prev;
            succs[i] = curr;
        }
        return key == curr->key;
    }

    // SHOULD NOT REACH HERE
    return false;

}

template<typename T>
bool SkipList<T>::search (T val) const {

    key_t key = myhash(val); 
    Node* prev = head;
    Node* curr = nullptr;
    Node* succ = nullptr;

    for (int i=MAX_LEVEL-1; i>=0; i--) {
        curr = prev->next[i].get_reference();
        while (true) {
            bool mark;
            succ = curr->next[i].get(mark);
            while (mark) {
                succ = succ->next[i].get(mark);
            }
            if (curr->key < key) {
                prev = curr;
                curr = succ;
            } else {
                break;
            }
        }
    }
    
    return key == curr->key;

}

template<typename T>
bool SkipList<T>::insert (T val) {

    int insertedHeight = randomHeight(MAX_LEVEL);
    std::vector<Node*> preds(insertedHeight); // index 0: lowest 
    std::vector<Node*> succs(insertedHeight); // index 0: lowest 

    while (true) {
        if (search_prev(val, preds, succs)) {
            return false;
        }

        Node* newNode = new Node(val, insertedHeight);
        for (int i=0; i<insertedHeight; i++) {
            newNode->next[i].set(succs[i], false);
        }

        Node* pred = preds[0];
        Node* succ = succs[0];
        if (!pred->next[0].CAS(succ, newNode, false, false)) {
            continue;
        }

        // connect
        for (int i=1; i<insertedHeight; i++) {
            while (true) {
                pred = preds[i];
                succ = succs[i];
                if (pred->next[i].CAS(succ, newNode, false, false)) break;
                search_prev(val, preds, succs);
            }
        }

        return true;

    }

    // SHOULD NOT REACH HERE!
    return false;
    
}

template<typename T>
bool SkipList<T>::remove (T val) {

    std::vector<Node*> preds(MAX_LEVEL); // index 0: lowest 
    std::vector<Node*> succs(MAX_LEVEL); // index 0: lowest 
    SkipList<T>::Node* succ;

    while (true) {

        if (!search_prev(val, preds, succs)) {
            return false;
        }

        bool mark;
        Node* victim = succs[0];
        for (int i=victim->height-1; i>=1; i--) {
            succ = victim->next[i].get(mark);
            while (!mark) {
                victim->next[i].CAS(succ, succ, false, true);
                succ = victim->next[i].get(mark);
            }
        }

        succ = victim->next[0].get(mark);

        while (true) {
            bool myMark = victim->next[0].CAS(succ, succ, false, true);
            succ = succs[0]->next[0].get(mark);
            if (myMark) {
                search_prev(val, preds, succs);
                return true;
            } else if (mark) {
                return false;
            }
        }

    }

    // SHOULD NOT REACH HERE!
    return false;

}

template<typename T>
void SkipList<T>::printList () const {


    // find the offsets for printing based on level 0
    std::vector<std::pair<int, Node*>> offsets;
    Node* curr = this->head->next[0].get_reference();
    int offset = 0;
    while (curr != this->tail) {
        offsets.push_back(std::make_pair(offset, curr));
        std::string itemstr = curr->toStr();
        offset += static_cast<int>(itemstr.length() + 1);
        curr = curr->next[0].get_reference();
    }

    int numOffsets = static_cast<int>(offsets.size());
    int numRight = static_cast<int>(this->head->next.size());
    while (numRight > 0 && this->head->next[--numRight].get_reference() == this->tail) {}

    for (int i=numRight; i>=0; i--) { // horizontal
        curr = this->head->next[i].get_reference();
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
                curr = curr->next[i].get_reference();
                p = j;
                prevLength = static_cast<int>(itemstr.length());
            } 
        }
        printNewLine();
    }
    
}

template<typename T>
bool SkipList<T>::empty () const {

    return this->head->next[0].get_reference() == this->tail;
}


// force compilier to instantiate T=int
template class SkipList<int>;