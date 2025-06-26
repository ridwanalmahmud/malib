/**
 * @file vector.h
 * @brief Includes core vector functionalities
 * @date 25/06/25
 */

#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

#define VECTOR_MIN_CAPACITY 16
#define VECTOR_GROWTH_FACTOR 2

typedef enum {
    VECTOR_SUCCESS = 0,
    VECTOR_ERROR_NULL,
    VECTOR_ERROR_MEM,
    VECTOR_ERROR_INIT,
    VECTOR_ERROR_SIZE,
    VECTOR_ERROR_MATH,
    VECTOR_ERROR_INDEX,
    VECTOR_ERROR_INVALID_ARG,
    VECTOR_ERROR_READONLY
} VectorError;

typedef struct {
    double_t *elements;
    size_t size;
    size_t capacity;
} Vector;

// --- Validation ---
bool vector_valid(const Vector *vector);

// --- Memory management ---
int vector_create(size_t size, Vector **out_vector);
int vector_init(Vector *vector, size_t size);
int vector_zero(Vector *vector);
int vector_resize_zero(Vector *vector, size_t size);
int vector_create_zero(size_t size, Vector **out_vector);
int vector_from_array(const double_t *arr, size_t size, Vector **out_vector);
int vector_copy(const Vector *src, Vector *dest);
int vector_free(Vector *vector);

// --- Capacity operations ---
int vector_resize(Vector *vector, size_t size);
int vector_reserve(Vector *vector, size_t capacity);
int vector_shrink_to_fit(Vector *vector);

// --- Element access ---
int vector_get(const Vector *vector, size_t index, double_t *out_val);
int vector_set(Vector *vector, size_t index, double_t val);
int vector_data(const Vector *vector, const double_t **out_data);
int vector_mutable_data(Vector *vector, double_t **out_data);
int vector_size(const Vector *vector, size_t *out_size);
int vector_capacity(const Vector *vector, size_t *out_capacity);

// --- Special vectors ---
int vector_2d(double_t x, double_t y, Vector **out_vector);
int vector_3d(double_t x, double_t y, double_t z, Vector **out_vector);
int vector_4d(double_t x,
              double_t y,
              double_t z,
              double_t w,
              Vector **out_vector);

// --- Vector arithmetic ---
int vector_add(const Vector *a, const Vector *b, Vector *result);
int vector_sub(const Vector *a, const Vector *b, Vector *result);
int vector_scale(const Vector *a, double_t scaler, Vector *result);
int vector_mult(const Vector *a, const Vector *b, Vector *result);
int vector_div(const Vector *a, const Vector *b, Vector *result);
int vector_negate(const Vector *a, Vector *result);

// --- Vector operations ---
int vector_dot(const Vector *a, const Vector *b, double_t result);
int vector_cross(const Vector *a, const Vector *b, double_t result);
int vector_magnitude(const Vector *vector);
int vector_normalize(const Vector *vector);
int vector_distance(const Vector *a, const Vector *b, double_t result);
int vector_angle(const Vector *a, const Vector *b, double_t result);

// --- Vector advanced operations ---
int vector_lerp(const Vector *a, const Vector *b, double_t t, Vector *result);
int vector_slerp(const Vector *a, const Vector *b, double_t t, Vector *result);
int vector_project(const Vector *a, const Vector *b, Vector *result);
int vector_reject(const Vector *a, const Vector *b, Vector *result);
int vector_reflect(const Vector *a, const Vector *b, Vector *result);

// --- Vector utility functions ---
int vector_min(const Vector *vector, double_t *min);
int vector_max(const Vector *vector, double_t *sum);
int vector_sum(const Vector *vector, double_t *sum);
int vector_mean(const Vector *vector, double_t *mean);
int vector_abs(Vector *vector);
int vector_floor(Vector *vector);
int vector_ceil(Vector *vector);
int vector_round(Vector *vector);
int vector_equals(const Vector *a, const Vector *b, double_t tolerance);
int vector_is_zero(const Vector *vector, double_t tolerance);
int vector_is_unit(const Vector *vector, double_t tolerance);
int vector_print(const Vector *vector);

// // --- Matrix-Vector operations ---
// int vector_mat_mult(const Matrix *matrix, const Vector *vector, Vector *result);
// int vector_transform(const Matrix *matrix, const Vector *vector);

#endif // !__VECTOR_H
