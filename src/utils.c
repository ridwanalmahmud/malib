/**
 * @file utils.c
 * @brief Optimized factorial implementation
 */

#include "utils.h"

bool factorial(int n, uint64_t* result) {
    // Input validation
    if (n < 0) {
        return false;
    }

    // Use LUT for known safe values
    if (n <= 20) {
        *result = FACTORIAL_LUT[n];
        return true;
    }

    // For larger values, compute with careful overflow checking
    uint64_t res = 1;
    for (int i = 2; i <= n; i++) {
        if (res > UINT64_MAX / i) {
            return false; // Overflow would occur
        }
        res *= i;
    }

    *result = res;
    return true;
}
