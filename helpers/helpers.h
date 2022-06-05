
#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdlib.h>

/*
@param maxHeight: height of the tallest node ("tower") in the skiplist. 
The maxHeight should be equal to head->next.size(), since the head
is the starting point for the skiplist
*/
int randomHeight (int maxHeight) {
    size_t height = 1;
    while ((float)rand() / RAND_MAX < .5f) {
        height++;
    }
    return std::min(static_cast<int>(height), maxHeight);
}

#endif