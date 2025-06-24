/**
 * @file utils.h
 * @brief Includes Necessary utilities functions
 * @author ridwanalmahmud
 * @date 22/06/25
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#include <stdbool.h>

// Maximum n before overflow (21! overflows uint64_t)
#define MAX_FACTORIAL_INPUT 20
// factorial flags
#define FLAG_FORCE_LOOKUP   0x01  // 00000001
#define FLAG_FORCE_GAMMA    0x02  // 00000010
#define FLAG_FORCE_STIRLING 0x04  // 00000100
#define FLAG_PREFER_EXACT   0x08  // 00001000

// Lookup table for 1! to 20! (fits in uint64_t)
static const uint64_t FACTORIAL_LUT[21] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800,
    39916800, 479001600, 6227020800, 87178291200, 1307674368000,
    20922789888000, 355687428096000, 6402373705728000,
    121645100408832000, 2432902008176640000
};

/**
 * @brief Computes factorial using a lookup table for optimal performance
 *
 * @param n Input value (0 <= n <= 20 for exact results)
 * @param[out] result Pointer to store the computed result
 * @return bool True if successful, false if overflow/invalid input
 *
 * @note Features:
 * - O(1) lookup time for n <= 20
 * - Hardcoded exact values for 0! to 20!
 * - Immediate rejection of invalid inputs
 *
 * @warning Returns false for:
 * - Negative inputs
 * - n > 20 (would overflow uint64_t)
 */
bool lut_factorial(int n, uint64_t* result);

/**
 * @brief Computes factorial using the gamma function for extended range
 * 
 * @param n Input value (n >= 0)
 * @param[out] result Pointer to store the computed result
 * @return bool True if successful, false on error/overflow
 *
 * @note Features:
 * - Uses tgamma() from math.h (Γ(n+1) = n!)
 * - Supports non-integer values through gamma function
 * - More precise than Stirling's approximation
 * - handles edge cases and overflow proection
 *
 * @warning:
 * - Slightly slower than lookup table
 * - Limited by floating-point precision
 * - Returns false for negative inputs
 */
bool gamma_factorial(double n, double* result);

/**
 * @brief Computes factorial approximation using Stirling's formula
 *
 * @param n Input value (n >= 0)
 * @param[out] result Pointer to store the approximated result
 * @return bool True if successful, false on invalid input
 *
 * @note Features:
 * - O(1) computation time
 * - Good approximation for n > 20
 * - Uses sqrt(2πn)*(n/e)^n*(1 + 1/(12n))
 *
 * @warning:
 * - Approximation error decreases as n increases
 * - Not suitable for exact integer results
 * - Returns false for negative inputs
 */
bool stirlings_factorial(double n, double* result);

/**
 * @brief Unified factorial dispatcher
 *
 * @param n Input number
 * @param result Output pointer
 * @param flags Control flags:
 *   - 0x01: Force lookup table
 *   - 0x02: Force gamma function
 *   - 0x04: Force Stirling's
 *   - 0x08: Prefer exact results
 * @return true on success
 */
bool factorial(double n, double* result, uint8_t flags);

#endif // !__UTILS_H
