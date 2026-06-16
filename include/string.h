#ifndef STRING_H
#define STRING_H

#include "type.h"

void itoa(int value, char* buffer);
int atoi(const char* buffer );
int strlen(char* text);
int strcmp(const char* a, const char* b);
void memset(void* ptr, char value, int size);
void memcpy(void* dst, const void* src, int size);
void strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);

#endif