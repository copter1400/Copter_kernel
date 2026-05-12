#ifndef TEST_H
#define TEST_H

typedef int (*function)();

void run_test(const char* name, function fn);
void warn(const char* name, int is_ok);

#endif