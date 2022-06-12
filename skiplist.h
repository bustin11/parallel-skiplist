
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <atomic>
#include <memory>

#include "helpers/helpers.h"

typedef int32_t key_t;
static std::hash<key_t> myhash;

template<typename T>
class SkipList {
    private:

    const static int MAX_LEVEL = 50;
    struct Node {

        key_t key;
        T val;
        uint16_t height;

        std::vector<AtomicMarkableReference<Node>> next;

        Node (T val, size_t height) : val(val), height(height){
            key = myhash(val); 
            next = std::vector<AtomicMarkableReference<Node>>(MAX_LEVEL);

            for (uint16_t i=0; i<height; i++) {
                next[i].set(nullptr, false);
            }
        }

        std::string toStr() {
            return std::to_string(val);
        }
    };

    Node* head;
    Node* tail;

    /*
    @param key: key (of an item) to lookup
    @param preds: stores the predecessors in the path to find the node 
    @param succs: stores the successors in the path to find the node (can
    contain the node of the key you are looking for.)

    @returns The first index found of the item. Since a skiplist is composed
    of 'sublists', every node has a varying height. The first index found is
    also its height - 1 (0 indexing). Returns -1 if not found
    */
    bool search_prev(T val,
        std::vector<Node*>& preds,
        std::vector<Node*>& succs);

    public:
    SkipList();
    ~SkipList();

    /*
    @param key: key (of an item) to be inserted

    @returns true if inserted, otherwise false
    */
    bool insert(T val);

    /*
    @param key: key (of an item) to be deleted

    @returns true if removed, otherwise false
    */
    bool remove(T val);

    /*
    @param key (of an item) to lookup

    @returns true if found, otherwise false
    */
    bool search(T val) const;

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