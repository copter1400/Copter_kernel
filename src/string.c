#include "string.h"
#include "type.h"

// change from integer to ascii
void itoa(int value, char* buffer) {

    int i = 0;
    int is_negative = 0;

    // if zero
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = 0;
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // write digits into buffer backwards
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    // null terminator
    buffer[i] = 0;

    // reverse
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}
// change from integer to ascii
int atoi(const char* buffer ) {
    int result = 0;
    int sign = 1;

    // skip spaces
    while (*buffer == ' ') buffer++;

    // sign
    if (*buffer == '-') {
        sign = -1;
        buffer++;
    } else if (*buffer == '+') {
        buffer++;
    }

    // convert digits
    while (*buffer >= '0' && *buffer <= '9') {
        result = result * 10 + (*buffer - '0');
        buffer++;
    }

    return result * sign;
}
// get size of string
int strlen(char* text) {
    int len = 0;
    while (text[len]) {
        len++;
    }
    return len;
}
// compare string
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
// set data
void memset(void* ptr, char value, int size) {

    char* p = ptr;

    for (int i = 0; i < size; i++) {
        p[i] = value;
    }
}
// copy data
void memcpy(void* dst, const void* src, int size) {

    char* d = dst;
    const char* s = src;

    for (int i = 0; i < size; i++) {
        d[i] = s[i];
    }
}
// copy data (end at terminator \0)
void strcpy(char* dst, const char* src) {

    int i = 0;

    while (src[i]) {

        dst[i] = src[i];

        i++;
    }

    dst[i] = 0;
}
// copy data (n char)
char* strncpy(char* dst, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    for (; i < n; i++) {
        dst[i] = '\0';   // pad with nulls
    }
    return dst;
}