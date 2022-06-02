
#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdlib.h>

int randomHeight (int maxHeight) {
    size_t height = 1;
    while ((float)rand() / RAND_MAX < .5f) {
        height++;
    }
    return std::min(static_cast<int>(height), maxHeight);
}

#endif