#include "math.h"
#include "type.h"

// Min
int32_t min(int32_t A, int32_t B) {
    if (A < B) {
        return A;
    }
    return B;
}
uint32_t umin(uint32_t A, uint32_t B) {
    if (A < B) {
        return A;
    }
    return B;
}
int64_t min64(int64_t A, int64_t B) {
    if (A < B) {
        return A;
    }
    return B;
}
uint64_t umin64(uint64_t A, uint64_t B) {
    if (A < B) {
        return A;
    }
    return B;
}

// Max
int32_t max(int32_t A, int32_t B) {
    if (A > B) {
        return A;
    }
    return B;
}
uint32_t umax(uint32_t A, uint32_t B) {
    if (A > B) {
        return A;
    }
    return B;
}
int64_t max64(int64_t A, int64_t B) {
    if (B > A) {
        return B;
    }
    return A;
}
uint64_t umax64(uint64_t A, uint64_t B) {
    if (B > A) {
        return B;
    }
    return A;
}

// ABS
int32_t abs(int32_t A) {
    if (A < 0) {
        return -A;
    }
    return A;
}
int64_t abs64(int64_t A) {
    if (A < 0) {
        return -A;
    }
    return A;
}

// Clamp
int32_t clamp(int32_t A, int32_t ceil, int32_t floor) {
    if (A >= floor && A <= ceil) {
        return A;
    } else if (A > ceil) {
        return ceil;
    } else if (A < floor) {
        return floor;
    }
    return 0;
}
uint32_t uclamp(uint32_t A, uint32_t ceil, uint32_t floor) {
    if (A >= floor && A <= ceil) {
        return A;
    } else if (A > ceil) {
        return ceil;
    } else if (A < floor) {
        return floor;
    }
    return 0;
}
int64_t clamp64(int64_t A, int64_t ceil, int64_t floor) {
    if (A >= floor && A <= ceil) {
        return A;
    } else if (A > ceil) {
        return ceil;
    } else if (A < floor) {
        return floor;
    }
    return 0;
}
uint64_t uclamp64(uint64_t A, uint64_t ceil, uint64_t floor) {
    if (A >= floor && A <= ceil) {
        return A;
    } else if (A > ceil) {
        return ceil;
    } else if (A < floor) {
        return floor;
    }
    return 0;
}

// Sign
int sign(int32_t A) {
    if (A > 0) {
        return 1;
    } else if (A < 0) {
        return -1;
    } else if (A == 0) {
        return 0;
    }
    return 0;
}
int sign64(int64_t A) {
    if (A > 0) {
        return 1;
    } else if (A < 0) {
        return -1;
    } else if (A == 0) {
        return 0;
    }
    return 2;
}

// isqure
uint32_t isqrt(uint32_t A) {
    uint32_t x = 0;

    while ((x+1) * (x+1) <= A) {
        x++;
    }

    return x;
}
uint64_t isqrt64(uint64_t A) {
    uint64_t x = 0;

    while ((x+1) * (x+1) <= A) {
        x++;
    }

    return x;
}
