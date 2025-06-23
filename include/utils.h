/**
 * @file factorial.h
 * @brief Optimized factorial computation using only standard C
 * @author ridwanalmahmud
 * @date 22/06/25
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#include <stdbool.h>

// Maximum n before overflow (21! overflows uint64_t)
#define MAX_FACTORIAL_INPUT 20

// Lookup table for 1! to 20! (fits in uint64_t)
static const uint64_t FACTORIAL_LUT[21] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800,
    39916800, 479001600, 6227020800, 87178291200, 1307674368000,
    20922789888000, 355687428096000, 6402373705728000,
    121645100408832000, 2432902008176640000
};

/**
 * @brief Computes factorial with multiple optimizations
 *
 * @param n Input value (0 <= n <= 20 for exact results)
 * @param[out] result Pointer to store the result
 * @return bool True if successful, false if overflow/error
 *
 * @note Uses:
 * - Lookup table for n <= 20
 * - Loop unrolling
 * - Early overflow checking
 * @warning Returns false for n > 20 (would overflow uint64_t)
 */
bool factorial(int n, uint64_t* result);

#endif // !__UTILS_H
