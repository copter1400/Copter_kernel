#ifndef MATH_H
#define MATH_H

#include "type.h"

// min
int32_t  min(int32_t A, int32_t B);
uint32_t umin(uint32_t A, uint32_t B);
int64_t  min64(int64_t A, int64_t B);
uint64_t umin64(uint64_t A, uint64_t B);

// max
int32_t  max(int32_t A, int32_t B);
uint32_t umax(uint32_t A, uint32_t B);
int64_t  max64(int64_t A, int64_t B);
uint64_t umax64(uint64_t A, uint64_t B);

// abs
int32_t abs(int32_t A);
int64_t abs64(int64_t A);

// clamp
int32_t  clamp(int32_t A, int32_t ceil, int32_t floor);
uint32_t uclamp(uint32_t A, uint32_t ceil, uint32_t floor);
int64_t  clamp64(int64_t A, int64_t ceil, int64_t floor);
uint64_t uclamp64(uint64_t A, uint64_t ceil, uint64_t floor);

// sign
int sign(int32_t A);
int sign64(int64_t A);

// isqrt
uint32_t isqrt(uint32_t A);
uint64_t isqrt64(uint64_t A);


#endif