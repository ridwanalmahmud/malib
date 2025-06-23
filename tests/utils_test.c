#include "unity.h"
#include <stdint.h>
#include "utils.h"

// Test suite setup and tear down
void setUp(void) {
    // This is run before each test
}

void tearDown(void) {
    // This is run after each test
}

// ---- Exact Results (LUT) ----
void test_factorial_lut_exact_values(void) {
    uint64_t result;

    // Test 0! to 20! against known values
    for (uint8_t n = 0; n <= 20; n++) {
        TEST_ASSERT_TRUE_MESSAGE(factorial(n, &result),
                "Factorial should succeed");
        TEST_ASSERT_EQUAL_UINT64_MESSAGE(FACTORIAL_LUT[n], result,
                "Factorial value mismatch");
    }
}

// ---- Edge Cases ----
void test_factorial_edge_cases(void) {
    uint64_t result;

    // 0! = 1 (special case)
    TEST_ASSERT_TRUE(factorial(0, &result));
    TEST_ASSERT_EQUAL_UINT64(1, result);

    // 1! = 1
    TEST_ASSERT_TRUE(factorial(1, &result));
    TEST_ASSERT_EQUAL_UINT64(1, result);
}

// ---- Overflow Protection ----
void test_factorial_overflow_handling(void) {
    uint64_t result;

    // 21! should fail (overflow)
    TEST_ASSERT_FALSE_MESSAGE(factorial(21, &result),
            "21! should overflow uint64_t");

    // Verify result unchanged on failure
    uint64_t prev = 0xDEADBEEF;
    result = prev;
    TEST_ASSERT_FALSE(factorial(255, &result));
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(prev, result, 
            "Result buffer should be unchanged on failure");
}

// ---- Input Validation ----
void test_factorial_invalid_input(void) {
    uint64_t result;

    // Negative input (should fail cleanly)
    TEST_ASSERT_FALSE(factorial(-1, &result));

    // Input > 255 (uint8_t max)
    TEST_ASSERT_FALSE(factorial(256, &result));

    // Test overflow case
    TEST_ASSERT_FALSE(factorial(21, &result));
}

// Test runner
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_factorial_lut_exact_values);
    RUN_TEST(test_factorial_edge_cases);
    RUN_TEST(test_factorial_overflow_handling);
    RUN_TEST(test_factorial_invalid_input);

    return UNITY_END();
}
