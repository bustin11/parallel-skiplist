
#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdlib.h>

/*
@param maxHeight: height of the tallest node ("tower") in the skiplist. 
The maxHeight should be equal to head->next.size(), since the head
is the starting point for the skiplist

inline to prevent multiple defintion
*/
inline int randomHeight (int maxHeight) {
    size_t height = 1;
    while ((float)rand() / RAND_MAX < .5f) {
        height++;
    }
    return std::min(static_cast<int>(height), maxHeight);
}

template<typename T>
struct alignas(8) MarkableReference {

    T* val = nullptr; 
    bool mark = false; // 1 byte
    // PADDING is needed because compare and exchange 
    // compares all 8bytes
    char PADDING[3]; // 3 bytes

    // need default constructor for atomic operations
    MarkableReference() {
        val = nullptr;
        mark = false;
        for (int i=0; i<3; i++) PADDING[i] = '\0';
    }
    MarkableReference (T* val, bool mark) : val(val), mark(mark) {
        for (int i=0; i<3; i++) PADDING[i] = '\0';
    }

};


template<typename T>
class AtomicMarkableReference {
    private:
    std::atomic<MarkableReference<T>> myRef;

    public:
    // need default constructor for atomic operations
    AtomicMarkableReference () {
        myRef = MarkableReference<T>(nullptr, false);
    }
    AtomicMarkableReference (T* val, bool mark) {
        myRef = MarkableReference<T>(val, mark);
    }

    T* get(bool& mark) {
        MarkableReference<T> ref = myRef.load();
        mark = ref.mark;
        return ref.val;
    }

    T* get_reference() {
        return myRef.load().val;
    }

    void set(T* newVal, bool newMark) {
        myRef.store(MarkableReference<T>(newVal, newMark));
    }

    void set_marked (bool newMark) {
        MarkableReference<T>* ref = myRef.load();
        T* val = ref.val;
        ref.store(MarkableReference<T>(ref.val, newMark));
    } 

    bool CAS (T* expectedVal, T* desiredVal, 
             bool expectedMark, bool desiredMark) {
        
        MarkableReference<T> ref = 
            MarkableReference<T>(expectedVal, expectedMark);
        return myRef.compare_exchange_strong(
                ref,
                MarkableReference<T>(desiredVal, desiredMark)
            );
    }

};


#endif