/**
 * @file utils.c
 * @brief Comprehensive factorial computation with multiple methods
 * @date 22/06/25
 */

#include "utils.h"

/**
 * @brief Exact factorial using lookup table (0-20)
 */
bool lookup_factorial(int n, uint64_t* result) {
    if (n < 0 || n > 20) return false;
    *result = FACTORIAL_LUT[n];
    return true;
}

/**
 * @brief Factorial via gamma function (supports non-integers)
 */
bool gamma_factorial(double_t n, double_t* result) {
    if (n < 0 || isnan(n)) return false;
    if (n == 0) { *result = 1.0; return true; }
    if (n > 170) { *result = INFINITY; return false; }

    *result = tgamma(n + 1.0);
    return !isinf(*result) && !isnan(*result);
}

/**
 * @brief Factorial approximation using Stirling's formula
 */
bool stirlings_factorial(double_t n, double_t* result) {
    if (n < 0 || isnan(n)) return false;
    if (n == 0 || n == 1) { *result = 1.0; return true; }

    const double_t pi = 3.14159265358979323846;
    double_t term1 = sqrt(2 * pi * n);
    double_t term2 = pow(n / exp(1.0), n);
    double_t term3 = 1.0 + (1.0 / (12.0 * n));

    *result = term1 * term2 * term3;
    return true;
}

/**
 * @brief Unified factorial dispatcher
 */
bool factorial(double_t n, double_t* result, uint8_t flags) {
    // Common validation
    if (n < 0 || isnan(n)) return false;

    // Forced method cases
    if (flags & FLAG_FORCE_LOOKUP) {
        int int_n = (int)n;
        if (int_n != n) return false;
        uint64_t exact;
        if (!lookup_factorial(int_n, &exact)) return false;
        *result = (double_t)exact;
        return true;
    }
    if (flags & FLAG_FORCE_GAMMA) return gamma_factorial(n, result);
    if (flags & FLAG_FORCE_STIRLING) return stirlings_factorial(n, result);

    // Automatic selection
    if ((flags & FLAG_PREFER_EXACT) || n == (int)n) {
        int int_n = (int)n;

        // Exact integer cases
        if (int_n <= 20) {
            uint64_t exact;
            if (lookup_factorial(int_n, &exact)) {
                *result = (double_t)exact;
                return true;
            }
        }

        // Larger but still exact in double_t
        if (int_n <= 170) {
            return gamma_factorial(n, result);
        }

        // Too large for exact representation
        if (flags & FLAG_PREFER_EXACT) return false;
    }

    // Fallback to Stirling's
    return stirlings_factorial(n, result);
}
