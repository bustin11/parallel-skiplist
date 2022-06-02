
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <atomic>
#include <memory>

#include "omp.h"
#if OMP==0
#define omp_init_lock
#define omp_set_lock
#define omp_unset_lock
#define omp_destroy_lock
#define omp_test_lock
#endif

typedef int32_t key_t;

// std::vector<Node*> left;
class Node {
    private:
    int state; // marked (logically removed) or 
                // fully linked (all connections established)

    public:
    int height;
    std::vector<std::shared_ptr<Node>> next;
    key_t key;
    omp_lock_t lock;

    ~Node() {
        omp_destroy_lock(&lock);
    }

    Node(key_t key, int height) {
        state = 0;
        omp_init_lock(&lock);
        this->key = key;
        this->height = height;
        this->next = std::vector<std::shared_ptr<Node>>(height, nullptr);
    }

    std::string toStr() {
        return std::to_string(key).c_str();
    }

    void set_fully_linked() {
        state = state | 1;
    }

    void unset_fully_linked() {
        state = state & 2;
    }

    void set_marked() {
        state = state | 2;
    }

    void unset_marked() {
        state = state & 1;
    }

    bool is_fully_linked() {
        return state & 1;
    }

    bool is_marked() {
        return state & 2;
    }
};

class SkipList {
    private:
    int search_prev(key_t key, 
    std::vector<std::shared_ptr<Node>>& preds,
    std::vector<std::shared_ptr<Node>>& succs) const;
    std::shared_ptr<Node> head; // very left
    const int MAX_LEVEL = 50;

    public:
    SkipList();
    ~SkipList();

    bool insert(key_t key);
    bool remove(key_t key);
    bool search(key_t key) const;
    bool empty() const;
    
    void printList() const;
};

#endif