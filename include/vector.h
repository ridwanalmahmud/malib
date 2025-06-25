/**
 * @file vector.h
 * @brief Includes core vector functionalities
 * @date 25/06/25
 */

#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    double_t *elements;
    size_t size;
    size_t capacity;
} Vector;

typedef enum {
    VECTOR_SUCCESS = 0,
    VECTOR_ERROR_NULL = -1,
    VECTOR_ERROR_SIZE = -2,
    VECTOR_ERROR_MEM = -3,
    VECTOR_ERROR_MATH = -4,
    VECTOR_INDEX = -5,
    VECTOR_ERROR_INIT = -6,
    VECTOR_ERROR_IO = -7,
} VectorError;

// --- Vector initialization ---
Vector *vector_create(size_t size);
int vector_init(Vector *vector, size_t size);
Vector *vector_zero(Vector *vector, size_t size);
Vector *vector_from_array(const double_t *arr, size_t size);
Vector *vector_copy(Vector *src, Vector *dest);

// --- Memory management ---
void vector_free(Vector *vector);
int vector_resize(Vector *vector, size_t size);
int vector_reserve(Vector *vector, size_t capacity);
int vector_shrink_to_fit(Vector *vector);

// --- Element access ---
int vector_get(const Vector *vector, size_t index, double_t *out_val);
int vector_set(Vector *vector, size_t index, double_t val);
const double_t *vector_data(const Vector *vector);
double_t *vector_mutable_data(Vector *vector);
size_t vetor_size(const Vector *vector);
size_t vector_capacity(const Vector *vector);

// --- Vector arithmetic ---
int vector_add(const Vector *a, const Vector *b, Vector *result);
int vector_sub(const Vector *a, const Vector *b, Vector *result);
int vector_scale(const Vector *a, double_t scaler, Vector *result);
int vector_mult(const Vector *a, const Vector *b, Vector *result);
int vector_div(const Vector *a, const Vector *b, Vector *result);
int vector_negate(const Vector *vector, Vector *result);

// --- Vector operations ---
int vector_dot(const Vector *a, const Vector *b, double_t result);
int vector_cross(const Vector *a, const Vector *b, double_t result);
int vector_norm(const Vector *vector, double_t result);
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
int vector_abs(const Vector *vector);
int vector_floor(const Vector *vector);
int vector_ceil(const Vector *vector);
int vector_round(const Vector *vector);
void vector_print(const Vector *vector);
bool vector_equals(const Vector *a, const Vector *b, double_t tolerance);
bool vector_is_zero(const Vector *vector, double_t tolerance);
bool vector_is_unit(const Vector *vector, double_t tolerance);
bool vector_valid(const Vector *vector);

// // --- Matrix-Vector operations ---
// int vector_mat_mult(const Matrix *matrix, const Vector *vector, Vector *result);
// int vector_transform(const Matrix *matrix, const Vector *vector);

#endif // !__VECTOR_H
