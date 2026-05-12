#include "string.h"

int strcmp(const char* a, const char* b) {

    int i = 0;

    while (a[i] && b[i]) {

        if (a[i] != b[i]) {
            return 1;
        }

        i++;
    }

    if (a[i] != b[i]) {
        return 1;
    }

    return 0;
}