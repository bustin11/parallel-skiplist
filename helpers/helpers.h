
#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdlib.h>

int randomHeight (int maxHeight) {
    int height = 1;
    while ((float)rand() / RAND_MAX < .5f) {
        height++;
    }
    return std::min(height, maxHeight);
}

#endif