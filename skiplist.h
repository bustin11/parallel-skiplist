
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <atomic>
#include <memory>

#include "omp.h"

typedef int32_t key_t;

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

// only support key insertions/removal operations
class SkipList {
    private:
    /*
    @param key: key (of an item) to lookup
    @param preds: stores the predecessors in the path to find the node 
    @param succs: stores the successors in the path to find the node (can
    contain the node of the key you are looking for.)

    @returns The first index found of the item. Since a skiplist is composed
    of 'sublists', every node has a varying height. The first index found is
    also its height - 1 (0 indexing). Returns -1 if not found
    */
    int search_prev(key_t key, 
    std::vector<std::shared_ptr<Node>>& preds,
    std::vector<std::shared_ptr<Node>>& succs) const;
    std::shared_ptr<Node> head; // very left

    const int MAX_LEVEL = 50;

    public:
    SkipList();

    /*
    @param key: key (of an item) to be inserted

    @returns true if inserted, otherwise false
    */
    bool insert(key_t key);

    /*
    @param key: key (of an item) to be deleted

    @returns true if removed, otherwise false
    */
    bool remove(key_t key);

    /*
    @param key (of an item) to lookup

    @returns true if found, otherwise false
    */
    bool search(key_t key) const;

    /*
    @returns true if skiplist is empty, otherwise false
    */
    bool empty() const;
    
    /*
    Pretty prints the skiplist. Reasonable ranges are from
    0-30. 
    */
    void printList() const;
};

#endif