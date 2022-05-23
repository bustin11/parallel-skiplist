
#include "skiplist.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>
#include <stack>
#include <functional>

#include "debug.h"

std::string item2str (item_t item) {
    return std::to_string(item);
}

std::string node2str (Node* node) {
    ASSERT(node);
    return item2str(node->item);
}

void printNode(Node* node) {
    printargs("%s", node2str(node).c_str());
}

SkipList::SkipList() {
    srand(time(NULL));
    this->head = new Node(); // starting dummy node
    this->head->right.push_back(nullptr);
}

SkipList::~SkipList() {

    Node* curr = this->head->right[0];
    delete this->head;
    while (curr) {
        Node* right = curr->right[0];
        delete curr;
        curr = right;
    }
}


void SkipList::search_prev (item_t item, std::stack<Node*>& predecessors) {

    int numRight = (int)(this->head->right.size());
    for (int i=numRight-1; i>=0; i--) {
        Node* curr = this->head; // top-left
        Node* currRight = curr->right[i];
        while (currRight && item >= currRight->item) {
            curr = currRight;
            currRight = curr->right[i];
        }
        predecessors.push(curr);
    }
    // printdebugfmt("pred.size()=%d", (int)predecessors.size())
}

bool SkipList::search (item_t item) {

    std::stack<Node*> A;
    search_prev(item, A);
    ASSERT(A.top());
    Node* curr = A.top();
    return curr && curr->item == item;
}


void SkipList::insert_with_height (item_t item, size_t height) {

    std::stack<Node*> predecessors; // NOTE: first element is the newest element
    search_prev(item, predecessors);
    ASSERT(predecessors.size() == this->head->right.size());
    int ancestor = 0;

    Node* hatNode = new Node();
    hatNode->item = item;
    do {
        // left and right
        Node* onTheLeft;
        Node* onTheRight;
        bool addingNewLevel = (ancestor >= (int) this->head->right.size());

        if (!predecessors.empty()) {
            onTheLeft = predecessors.top();
            predecessors.pop();
            onTheRight = !addingNewLevel ? 
                                onTheLeft->right[ancestor] : nullptr;
        } else {
            // this creates a new level
            onTheLeft = this->head;
            onTheRight = nullptr;
        }

        hatNode->left.push_back(onTheLeft);
        if (addingNewLevel) onTheLeft->right.push_back(hatNode);
        else onTheLeft->right[ancestor] = hatNode;

        hatNode->right.push_back(onTheRight);
        if (onTheRight && !addingNewLevel) onTheRight->left[ancestor] = hatNode;
        ancestor++;

    } while ((height > 0 && ancestor < (int)height) // deterministic
                    || (height == 0 && (float)rand() / RAND_MAX < .5f)); // random

    printdebugfmt("Inserted %s with height=%d", item2str(item).c_str(), ancestor);

}

void SkipList::insert (item_t item) {

    return insert_with_height(item, 0);
    
}


void SkipList::remove (item_t item) {
    std::stack<Node*> predecessors; // NOTE: first element is the newest element
    search_prev(item, predecessors);
    Node* curr = predecessors.top();
    if (curr && curr->item == item) {
        std::vector<Node*> onTheLeft = curr->left;
        std::vector<Node*> onTheRight = curr->right;
        for (int i=0; i< (int)onTheLeft.size(); i++) { // numLeft == numRight
            onTheLeft[i]->right[i] = onTheRight[i];
            if (onTheRight[i]) onTheRight[i]->left[i] = onTheLeft[i];
        }
    }
    delete curr;
}


void SkipList::printList (){

    // find the offsets for printing based on level 0
    std::vector<std::pair<int, Node*>> offsets;
    Node* curr = this->head->right[0];
    int offset = 0;
    while (curr) {
        offsets.push_back(std::make_pair(offset, curr));
        std::string itemstr = node2str(curr);
        offset += (int)(itemstr.length() + 1);
        curr = curr->right[0];
    }

    int numOffsets = (int) offsets.size();
    int numRight = (int) this->head->right.size();
\
    for (int i=numRight-1; i>=0; i--) { // horizontal
        curr = this->head->right[i];
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
                
                std::string itemstr = node2str(curr);
                printf("%s", itemstr.c_str());
                curr = curr->right[i];
                p = j;
                prevLength = (int)(itemstr.length());
            } 
        }
        printNewLine();
    }
}

bool SkipList::empty () {
    return (int)this->head->right.size() == 1;
}