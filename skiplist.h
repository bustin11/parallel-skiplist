
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <vector>
#include <stack>
#include <stdio.h>
#include <stdint.h>

#include "omp.h"

typedef int32_t item_t;

class Node {
    public:
    std::vector<Node*> left;
    std::vector<Node*> right;
    item_t item;
};

class SkipList {
    private:
    void search_prev(item_t id, std::stack<Node*>& predecessors) const;
    Node* head; // very left
    omp_lock_t lock;

    public:
    SkipList();
    ~SkipList();

    void insert(item_t item);
    void insert_with_height(item_t item, size_t height);
    void remove(item_t item);
    bool search(item_t item) const;
    bool empty();
    
    void printList();
};

#endif