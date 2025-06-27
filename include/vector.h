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

#define VECTOR_MIN_CAPACITY 16 ///< Minimum capacity when vector is created
#define VECTOR_GROWTH_FACTOR 2 ///< Growth factor when resizing vector

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

/**
 * @brief Vector structure containing elements and metadata
 *
 * The vector owns its elements array and is responsible for freeing it.
 */
typedef struct {
    double_t *elements; ///< Pointer to dynamically allocated array of elements
    size_t size; ///< Current number of elements in vector
    size_t capacity; ///< Currently allocated capacity of vector
} Vector;

// Section: Validation

/**
 * @brief Check if a vector is valid (non-null and has allocated elements)
 * @param vector Pointer to vector to check
 * @return true if vector is valid, false otherwise
 */
bool vector_valid(const Vector *vector);

// Section: Memory mangaement

/**
 * @brief Create a new vector with specified size
 * @param size Initial size of vector
 * @param[out] out_vector Pointer to receive newly created vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The caller owns the returned vector and must free it with vector_free()
 */
int vector_create(size_t size, Vector **out_vector);

/**
 * @brief Initialize an existing vector with specified size
 * @param vector Vector to initialize
 * @param size Initial size of vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note This will free any existing elements in the vector
 */
int vector_init(Vector *vector, size_t size);

/**
 * @brief Set all elements of vector to zero
 * @param vector Vector to zero
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_zero(Vector *vector);

/**
 * @brief Resize vector and zero all elements
 * @param vector Vector to resize
 * @param size New size for vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_resize_zero(Vector *vector, size_t size);

/**
 * @brief Create a new zero-initialized vector
 * @param size Size of vector to create
 * @param[out] out_vector Pointer to receive newly created vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The caller owns the returned vector and must free it with vector_free()
 */
int vector_create_zero(size_t size, Vector **out_vector);

/**
 * @brief Create vector from C array
 * @param arr Source array
 * @param size Number of elements in array
 * @param[out] out_vector Pointer to receive newly created vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The caller owns the returned vector and must free it with vector_free()
 */
int vector_from_array(const double_t *arr, size_t size, Vector **out_vector);

/**
 * @brief Copy contents of source vector to destination vector
 * @param src Source vector to copy from
 * @param dest Destination vector to copy to
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Destination vector will be resized to match source
 */
int vector_copy(const Vector *src, Vector *dest);

/**
 * @brief Free memory allocated by vector
 * @param vector Vector to free
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Frees both the vector structure and its elements
 * @note After calling, the vector pointer is no longer valid
 */
int vector_free(Vector *vector);

// Section: Capacity Operations

/**
 * @brief Resize vector to specified size
 * @param vector Vector to resize
 * @param size New size for vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note If growing, new elements are uninitialized
 */
int vector_resize(Vector *vector, size_t size);

/**
 * @brief Reserve capacity for vector
 * @param vector Vector to modify
 * @param capacity Minimum capacity to reserve
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Does not change vector size, only allocated capacity
 */
int vector_reserve(Vector *vector, size_t capacity);

/**
 * @brief Reduce capacity to match size
 * @param vector Vector to shrink
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_shrink_to_fit(Vector *vector);

// Section: Element Access

/**
 * @brief Get element at specified index
 * @param vector Vector to access
 * @param index Index of element to get
 * @param[out] out_val Pointer to receive element value
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_get(const Vector *vector, size_t index, double_t *out_val);

/**
 * @brief Set element at specified index
 * @param vector Vector to modify
 * @param index Index of element to set
 * @param val Value to set
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_set(Vector *vector, size_t index, double_t val);

/**
 * @brief Get pointer to underlying element array
 * @param vector Vector to access
 * @param[out] out_data Pointer to receive element array
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The returned pointer is owned by the vector and becomes invalid if the vector is resized or freed
 */
int vector_data(const Vector *vector, const double_t **out_data);

/**
 * @brief Get mutable pointer to underlying element array
 * @param vector Vector to access
 * @param[out] out_data Pointer to receive element array
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The returned pointer is owned by the vector and becomes invalid if the vector is resized or freed
 */
int vector_mutable_data(Vector *vector, double_t **out_data);

/**
 * @brief Get current size of vector
 * @param vector Vector to query
 * @param[out] out_size Pointer to receive size
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_size(const Vector *vector, size_t *out_size);

/**
 * @brief Get current capacity of vector
 * @param vector Vector to query
 * @param[out] out_capacity Pointer to receive capacity
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_capacity(const Vector *vector, size_t *out_capacity);

// Section: Special Vectors

/**
 * @brief Create 2D vector
 * @param x X component
 * @param y Y component
 * @param[out] out_vector Pointer to receive new vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 * 
 * @note The caller owns the returned vector and must free it with vector_free()
 */
int vector_2d(double_t x, double_t y, Vector **out_vector);

/**
 * @brief Create 3D vector
 * @param x X component
 * @param y Y component
 * @param z Z component
 * @param[out] out_vector Pointer to receive new vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The caller owns the returned vector and must free it with vector_free()
 */
int vector_3d(double_t x, double_t y, double_t z, Vector **out_vector);

/**
 * @brief Create 4D vector
 * @param x X component
 * @param y Y component
 * @param z Z component
 * @param w W component
 * @param[out] out_vector Pointer to receive new vector
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note The caller owns the returned vector and must free it with vector_free()
 */
int vector_4d(double_t x,
              double_t y,
              double_t z,
              double_t w,
              Vector **out_vector);

// Section: Vector Arithmetic

/**
 * @brief Element-wise vector addition (result = a + b)
 * @param a First operand
 * @param b Second operand
 * @param[out] result Vector to store result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_add(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Element-wise vector subtraction (result = a - b)
 * @param a First operand
 * @param b Second operand
 * @param[out] result Vector to store result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_sub(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Vector scaling (result = a * scalar)
 * @param a Vector to scale
 * @param scaler Scaling factor
 * @param[out] result Vector to store result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_scale(const Vector *a, double_t scaler, Vector *result);

/**
 * @brief Element-wise vector multiplication (result = a * b)
 * @param a First operand
 * @param b Second operand
 * @param[out] result Vector to store result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_mult(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Element-wise vector division (result = a / b)
 * @param a First operand
 * @param b Second operand
 * @param[out] result Vector to store result
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if any element of b is zero
 */
int vector_div(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Vector negation (result = -a)
 * @param a Vector to negate
 * @param[out] result Vector to store result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_negate(const Vector *a, Vector *result);

// Section: Vector Operations

/**
 * @brief Dot product of two vectors
 * @param a First vector
 * @param b Second vector
 * @param[out] result Pointer to store dot product result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_dot(const Vector *a, const Vector *b, double_t *result);

/**
 * @brief Cross product of two 3D vectors
 * @param a First vector (must be 3D)
 * @param b Second vector (must be 3D)
 * @param[out] result Vector to store result (must be 3D)
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_cross(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Compute magnitude (length) of vector
 * @param vector Vector to measure
 * @param[out] result Pointer to store magnitude
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_magnitude(const Vector *vector, double_t *result);

/**
 * @brief Normalize vector in-place (make unit length)
 * @param vector Vector to normalize
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if vector has zero length
 */
int vector_normalize(Vector *vector);

/**
 * @brief Compute Euclidean distance between two vectors
 * @param a First vector
 * @param b Second vector
 * @param[out] result Pointer to store distance
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_distance(const Vector *a, const Vector *b, double_t *result);

/**
 * @brief Compute angle between two vectors in radians
 * @param a First vector
 * @param b Second vector
 * @param[out] result Pointer to store angle
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if either vector has zero length
 */
int vector_angle(const Vector *a, const Vector *b, double_t *result);

// Section: Vector Advanced Operations

/**
 * @brief Linear interpolation between two vectors
 * @param a Start vector
 * @param b End vector
 * @param t Interpolation factor (0=a, 1=b)
 * @param[out] result Vector to store interpolated result
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_lerp(const Vector *a, const Vector *b, double_t t, Vector *result);

/**
 * @brief Spherical linear interpolation between two vectors
 * @param a Start vector (should be normalized)
 * @param b End vector (should be normalized)
 * @param t Interpolation factor (0=a, 1=b)
 * @param[out] result Vector to store interpolated result
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Falls back to linear interpolation if vectors are nearly parallel
 */
int vector_slerp(const Vector *a, const Vector *b, double_t t, Vector *result);

/**
 * @brief Project vector a onto vector b
 * @param a Vector to project
 * @param b Vector to project onto
 * @param[out] result Vector to store projection result
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if b has zero length
 */
int vector_project(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Reject vector a from vector b (component perpendicular to b)
 * @param a Vector to reject
 * @param b Vector to reject from
 * @param[out] result Vector to store rejection result
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if b has zero length
 */
int vector_reject(const Vector *a, const Vector *b, Vector *result);

/**
 * @brief Reflect vector a over vector b (like mirror reflection)
 * @param a Vector to reflect
 * @param b Vector to reflect over
 * @param[out] result Vector to store reflection result
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if b has zero length
 */
int vector_reflect(const Vector *a, const Vector *b, Vector *result);

// Section: Utility Functions

/**
 * @brief Find minimum element in vector
 * @param vector Vector to search
 * @param[out] min Pointer to store minimum value
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_min(const Vector *vector, double_t *min);

/**
 * @brief Find maximum element in vector
 * @param vector Vector to search
 * @param[out] max Pointer to store maximum value
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_max(const Vector *vector, double_t *max);

/**
 * @brief Compute sum of all vector elements
 * @param vector Vector to sum
 * @param[out] sum Pointer to store sum
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_sum(const Vector *vector, double_t *sum);

/**
 * @brief Compute mean of vector elements
 * @param vector Vector to analyze
 * @param[out] mean Pointer to store mean value
 * @return VECTOR_SUCCESS on success, error code otherwise
 *
 * @note Returns VECTOR_ERROR_MATH if vector is empty
 */
int vector_mean(const Vector *vector, double_t *mean);

/**
 * @brief Compute absolute value of all elements in-place
 * @param vector Vector to modify
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_abs(Vector *vector);

/**
 * @brief Apply floor function to all elements in-place
 * @param vector Vector to modify
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_floor(Vector *vector);

/**
 * @brief Apply ceiling function to all elements in-place
 * @param vector Vector to modify
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_ceil(Vector *vector);

/**
 * @brief Round all elements in-place
 * @param vector Vector to modify
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_round(Vector *vector);

/**
 * @brief Check if two vectors are equal within tolerance
 * @param a First vector
 * @param b Second vector
 * @param tolerance Maximum allowed difference per element
 * @return VECTOR_SUCCESS if equal within tolerance, error code otherwise
 */
int vector_equals(const Vector *a, const Vector *b, double_t tolerance);

/**
 * @brief Check if vector is approximately zero
 * @param vector Vector to check
 * @param tolerance Maximum allowed element magnitude
 * @return VECTOR_SUCCESS if all elements <= tolerance, error code otherwise
 */
int vector_is_zero(const Vector *vector, double_t tolerance);

/**
 * @brief Check if vector is approximately unit length
 * @param vector Vector to check
 * @param tolerance Maximum allowed deviation from 1.0
 * @return VECTOR_SUCCESS if |magnitude-1| <= tolerance, error code otherwise
 */
int vector_is_unit(const Vector *vector, double_t tolerance);

/**
 * @brief Print vector contents to stdout
 * @param vector Vector to print
 * @return VECTOR_SUCCESS on success, error code otherwise
 */
int vector_print(const Vector *vector);

// // --- Matrix-Vector operations ---
// int vector_mat_mult(const Matrix *matrix, const Vector *vector, Vector *result);
// int vector_transform(const Matrix *matrix, const Vector *vector);

#endif // !__VECTOR_H
