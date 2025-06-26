/**
 * @file vector.c
 * @brief Vector computation
 * @date 25/06/25
 */

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool vector_valid(const Vector *vector) {
    return (vector != NULL && vector->elements != NULL);
}

static size_t calculate_new_capacity(size_t current) {
    if (current < VECTOR_MIN_CAPACITY) {
        return VECTOR_MIN_CAPACITY;
    }
    return (current * VECTOR_GROWTH_FACTOR);
}

// --- Vector initialization ---

int vector_create(size_t size, Vector **out_vector) {
    if (!out_vector)
        return VECTOR_ERROR_NULL;

    Vector *vector = malloc(sizeof(Vector));
    if (!vector)
        return VECTOR_ERROR_MEM;

    if (size == 0) {
        vector->elements = NULL;
    } else {
        vector->elements = calloc(size, sizeof(double_t));
        if (!vector->elements) {
            free(vector);
            return VECTOR_ERROR_MEM;
        }
    }

    vector->size = size;
    vector->capacity = size;
    *out_vector = vector;
    return VECTOR_SUCCESS;
}

int vector_init(Vector *vector, size_t size) {
    if (!vector)
        return VECTOR_ERROR_NULL;

    double_t *elements = size > 0 ? calloc(size, sizeof(double_t)) : NULL;
    if (size > 0 && !elements)
        return VECTOR_ERROR_MEM;

    free(vector->elements);
    vector->elements = elements;
    vector->size = size;
    vector->capacity = size;
    return VECTOR_SUCCESS;
}

int vector_zero(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector->elements)
        return VECTOR_ERROR_INIT;

    memset(vector->elements, 0, vector->size * sizeof(double_t));
    return VECTOR_SUCCESS;
}

int vector_resize_zero(Vector *vector, size_t size) {
    if (!vector)
        return VECTOR_ERROR_NULL;

    int resize_result = vector_resize(vector, size);
    if (resize_result != VECTOR_SUCCESS) {
        return resize_result;
    }

    memset(vector->elements, 0, size * sizeof(double_t));
    return VECTOR_SUCCESS;
}

int vector_create_zero(size_t size, Vector **out_vector) {
    int create_result = vector_create(size, out_vector);
    if (create_result != VECTOR_SUCCESS) {
        return create_result;
    }
    return vector_zero(*out_vector);
}

int vector_from_array(const double_t *arr, size_t size, Vector **out_vector) {
    if (!arr || !out_vector)
        return VECTOR_ERROR_NULL;

    int create_result = vector_create(size, out_vector);
    if (create_result != VECTOR_SUCCESS) {
        return create_result;
    }

    memcpy((*out_vector)->elements, arr, size * sizeof(double_t));
    return VECTOR_SUCCESS;
}

int vector_copy(const Vector *src, Vector *dest) {
    if (!src || !dest)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(src))
        return VECTOR_ERROR_INIT;

    int init_result = vector_init(dest, src->size);
    if (init_result != VECTOR_SUCCESS) {
        return init_result;
    }

    memcpy(dest->elements, src->elements, src->size * sizeof(double_t));
    return VECTOR_SUCCESS;
}

int vector_free(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;

    free(vector->elements);
    free(vector);
    return VECTOR_SUCCESS;
}

// --- Memory management ---

int vector_resize(Vector *vector, size_t size) {
    if (!vector)
        return VECTOR_ERROR_NULL;

    if (size <= vector->capacity) {
        vector->size = size;
        return VECTOR_SUCCESS;
    }

    size_t new_capacity = calculate_new_capacity(size);
    double_t *new_elements =
        realloc(vector->elements, new_capacity * sizeof(double_t));
    if (!new_elements)
        return VECTOR_ERROR_MEM;

    if (size > vector->size) {
        memset(new_elements + vector->size,
               0,
               (size - vector->size) * sizeof(double_t));
    }

    vector->elements = new_elements;
    vector->size = size;
    vector->capacity = new_capacity;
    return VECTOR_SUCCESS;
}

int vector_reserve(Vector *vector, size_t capacity) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (capacity <= vector->capacity)
        return VECTOR_SUCCESS;

    double_t *new_elements =
        realloc(vector->elements, capacity * sizeof(double_t));
    if (!new_elements)
        return VECTOR_ERROR_MEM;

    vector->elements = new_elements;
    vector->capacity = capacity;
    return VECTOR_SUCCESS;
}

int vector_shrink_to_fit(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector->elements)
        return VECTOR_ERROR_INIT;

    if (vector->size == 0) {
        free(vector->elements);
        vector->elements = NULL;
        vector->capacity = 0;
        return VECTOR_SUCCESS;
    }

    if (vector->size == vector->capacity)
        return VECTOR_SUCCESS;

    double_t *new_elements =
        realloc(vector->elements, vector->size * sizeof(double_t));
    if (!new_elements)
        return VECTOR_ERROR_MEM;

    vector->elements = new_elements;
    vector->capacity = vector->size;
    return VECTOR_SUCCESS;
}

// --- Element access ---

int vector_get(const Vector *vector, size_t index, double_t *out_val) {
    if (!vector || !out_val)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;
    if (index >= vector->size)
        return VECTOR_ERROR_INDEX;

    *out_val = vector->elements[index];
    return VECTOR_SUCCESS;
}

int vector_set(Vector *vector, size_t index, double_t val) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;
    if (index >= vector->size)
        return VECTOR_ERROR_INDEX;

    vector->elements[index] = val;
    return VECTOR_SUCCESS;
}

int vector_data(const Vector *vector, const double_t **out_data) {
    if (!vector || !out_data)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    *out_data = vector->elements;
    return VECTOR_SUCCESS;
}

int vector_mutable_data(Vector *vector, double_t **out_data) {
    if (!vector || !out_data)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    *out_data = vector->elements;
    return VECTOR_SUCCESS;
}

int vector_size(const Vector *vector, size_t *out_size) {
    if (!vector || !out_size)
        return VECTOR_ERROR_NULL;
    *out_size = vector->size;
    return VECTOR_SUCCESS;
}

int vector_capacity(const Vector *vector, size_t *out_capacity) {
    if (!vector || !out_capacity)
        return VECTOR_ERROR_NULL;
    *out_capacity = vector->capacity;
    return VECTOR_SUCCESS;
}

// --- Special Vectors ---
int vector_2d(double_t x, double_t y, Vector **out_vector) {
    if (!out_vector)
        return VECTOR_ERROR_NULL;

    int create_result = vector_create(2, out_vector);
    if (create_result != VECTOR_SUCCESS) {
        return create_result;
    }

    (*out_vector)->elements[0] = x;
    (*out_vector)->elements[1] = y;
    return VECTOR_SUCCESS;
}

int vector_3d(double_t x, double_t y, double_t z, Vector **out_vector) {
    if (!out_vector)
        return VECTOR_ERROR_NULL;

    int create_result = vector_create(3, out_vector);
    if (create_result != VECTOR_SUCCESS) {
        return create_result;
    }

    (*out_vector)->elements[0] = x;
    (*out_vector)->elements[1] = y;
    (*out_vector)->elements[2] = z;
    return VECTOR_SUCCESS;
}

int vector_4d(double_t x,
              double_t y,
              double_t z,
              double_t w,
              Vector **out_vector) {
    if (!out_vector)
        return VECTOR_ERROR_NULL;

    int create_result = vector_create(4, out_vector);
    if (create_result != VECTOR_SUCCESS) {
        return create_result;
    }

    (*out_vector)->elements[0] = x;
    (*out_vector)->elements[1] = y;
    (*out_vector)->elements[2] = z;
    (*out_vector)->elements[3] = w;
    return VECTOR_SUCCESS;
}

// --- Vector operations ---
int vector_add(const Vector *a, const Vector *b, Vector *result) {
    if (!a || !b || !result)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(b) || !vector_valid(result))
        return VECTOR_ERROR_INIT;
    if (a->size != b->size || a->size != result->size)
        return VECTOR_ERROR_SIZE;

    for (size_t i = 0; i < a->size; i++) {
        result->elements[i] = a->elements[i] + b->elements[i];
    }
    return VECTOR_SUCCESS;
}

int vector_sub(const Vector *a, const Vector *b, Vector *result) {
    if (!a || !b || !result)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(b) || !vector_valid(result))
        return VECTOR_ERROR_INIT;
    if (a->size != b->size || a->size != result->size)
        return VECTOR_ERROR_SIZE;

    for (size_t i = 0; i < a->size; i++) {
        result->elements[i] = a->elements[i] - b->elements[i];
    }
    return VECTOR_SUCCESS;
}

int vector_scale(const Vector *a, double_t scaler, Vector *result) {
    if (!a || !result)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(result))
        return VECTOR_ERROR_INIT;
    if (a->size != result->size)
        return VECTOR_ERROR_SIZE;

    for (size_t i = 0; i < a->size; i++) {
        result->elements[i] = a->elements[i] * scaler;
    }
    return VECTOR_SUCCESS;
}

int vector_mult(const Vector *a, const Vector *b, Vector *result) {
    if (!a || !b || !result)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(b) || !vector_valid(result))
        return VECTOR_ERROR_INIT;
    if (a->size != b->size || a->size != result->size)
        return VECTOR_ERROR_SIZE;

    for (size_t i = 0; i < a->size; i++) {
        result->elements[i] = a->elements[i] * b->elements[i];
    }
    return VECTOR_SUCCESS;
}

int vector_div(const Vector *a, const Vector *b, Vector *result) {
    if (!a || !b || !result)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(b) || !vector_valid(result))
        return VECTOR_ERROR_INIT;
    if (a->size != b->size || a->size != result->size)
        return VECTOR_ERROR_SIZE;

    for (size_t i = 0; i < a->size; i++) {
        if (b->elements[i] == 0.0)
            return VECTOR_ERROR_MATH;
        result->elements[i] = a->elements[i] / b->elements[i];
    }
    return VECTOR_SUCCESS;
}

int vector_negate(const Vector *a, Vector *result) {
    if (!a || !result)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(result))
        return VECTOR_ERROR_INIT;
    if (a->size != result->size)
        return VECTOR_ERROR_SIZE;

    for (size_t i = 0; i < a->size; i++) {
        result->elements[i] = -a->elements[i];
    }
    return VECTOR_SUCCESS;
}

// --- Vector utility function ---

int vector_min(const Vector *vector, double_t *min) {
    if (!vector || !min)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;
    if (vector->size == 0)
        return VECTOR_ERROR_SIZE;

    const double_t *data = vector->elements;
    double_t current_min = data[0];
    size_t i = 1;

    // Process 4 elements at a time
    for (; i + 3 < vector->size; i += 4) {
        current_min = fmin(current_min, data[i]);
        current_min = fmin(current_min, data[i + 1]);
        current_min = fmin(current_min, data[i + 2]);
        current_min = fmin(current_min, data[i + 3]);
    }

    // Process remaining elements
    for (; i < vector->size; i++) {
        current_min = fmin(current_min, data[i]);
    }

    *min = current_min;
    return VECTOR_SUCCESS;
}

int vector_max(const Vector *vector, double_t *max) {
    if (!vector || !max)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;
    if (vector->size == 0)
        return VECTOR_ERROR_SIZE;

    const double_t *data = vector->elements;
    double_t current_max = data[0];
    size_t i = 1;

    for (; i + 3 < vector->size; i += 4) {
        current_max = fmax(current_max, data[i]);
        current_max = fmax(current_max, data[i + 1]);
        current_max = fmax(current_max, data[i + 2]);
        current_max = fmax(current_max, data[i + 3]);
    }

    for (; i < vector->size; i++) {
        current_max = fmax(current_max, data[i]);
    }

    *max = current_max;
    return VECTOR_SUCCESS;
}

int vector_sum(const Vector *vector, double_t *sum) {
    if (!vector || !sum)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    const double_t *data = vector->elements;
    double_t total = 0.0;
    double_t c = 0.0; // Compensation for lost low-order bits

    for (size_t i = 0; i < vector->size; i++) {
        double_t y = data[i] - c;
        double_t t = total + y;
        c = (t - total) - y;
        total = t;
    }

    *sum = total;
    return VECTOR_SUCCESS;
}

int vector_mean(const Vector *vector, double_t *mean) {
    if (!vector || !mean)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;
    if (vector->size == 0)
        return VECTOR_ERROR_SIZE;

    double_t sum;
    int sum_result = vector_sum(vector, &sum);
    if (sum_result != VECTOR_SUCCESS) {
        return sum_result;
    }

    *mean = sum / vector->size;
    return VECTOR_SUCCESS;
}

int vector_abs(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    double_t *data = vector->elements;
    const size_t size = vector->size;

    // Process 4 elements at a time for better pipelining
    size_t i = 0;
    for (; i + 3 < size; i += 4) {
        data[i] = fabs(data[i]);
        data[i + 1] = fabs(data[i + 1]);
        data[i + 2] = fabs(data[i + 2]);
        data[i + 3] = fabs(data[i + 3]);
    }

    for (; i < size; i++) {
        data[i] = fabs(data[i]);
    }

    return VECTOR_SUCCESS;
}

int vector_floor(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    double_t *data = vector->elements;
    const size_t size = vector->size;

    // Unrolled loop
    size_t i = 0;
    for (; i + 3 < size; i += 4) {
        data[i] = floor(data[i]);
        data[i + 1] = floor(data[i + 1]);
        data[i + 2] = floor(data[i + 2]);
        data[i + 3] = floor(data[i + 3]);
    }

    // Remainder
    for (; i < size; i++) {
        data[i] = floor(data[i]);
    }

    return VECTOR_SUCCESS;
}

int vector_ceil(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    double_t *data = vector->elements;
    const size_t size = vector->size;

    // Process chunks of 4
    size_t i = 0;
    for (; i + 3 < size; i += 4) {
        data[i] = ceil(data[i]);
        data[i + 1] = ceil(data[i + 1]);
        data[i + 2] = ceil(data[i + 2]);
        data[i + 3] = ceil(data[i + 3]);
    }

    // Process remainder
    for (; i < size; i++) {
        data[i] = ceil(data[i]);
    }

    return VECTOR_SUCCESS;
}

// Optimized in-place rounding
int vector_round(Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    double_t *data = vector->elements;
    const size_t size = vector->size;

    // SIMD-friendly processing
    size_t i = 0;
    for (; i + 3 < size; i += 4) {
        data[i] = round(data[i]);
        data[i + 1] = round(data[i + 1]);
        data[i + 2] = round(data[i + 2]);
        data[i + 3] = round(data[i + 3]);
    }

    // Final elements
    for (; i < size; i++) {
        data[i] = round(data[i]);
    }

    return VECTOR_SUCCESS;
}

int vector_equals(const Vector *a, const Vector *b, double_t tolerance) {
    if (!a || !b)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(a) || !vector_valid(b))
        return VECTOR_ERROR_INIT;
    if (a->size != b->size)
        return VECTOR_ERROR_SIZE;

    const double_t *a_data = a->elements;
    const double_t *b_data = b->elements;

    for (size_t i = 0; i < a->size; i++) {
        if (fabs(a_data[i] - b_data[i]) > tolerance) {
            return VECTOR_SUCCESS; // Return success but result is false
        }
    }

    return VECTOR_SUCCESS;
}

int vector_is_zero(const Vector *vector, double_t tolerance) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    const double_t *data = vector->elements;

    for (size_t i = 0; i < vector->size; i++) {
        if (fabs(data[i]) > tolerance) {
            return VECTOR_SUCCESS; // Return success but result is false
        }
    }

    return VECTOR_SUCCESS;
}

int vector_is_unit(const Vector *vector, double_t tolerance) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    double_t sum_of_squares = 0.0;
    const double_t *data = vector->elements;

    for (size_t i = 0; i < vector->size; i++) {
        sum_of_squares += data[i] * data[i];
    }

    return (fabs(sum_of_squares - 1.0) <= tolerance) ? VECTOR_SUCCESS
                                                     : VECTOR_ERROR_MATH;
}

int vector_print(const Vector *vector) {
    if (!vector)
        return VECTOR_ERROR_NULL;
    if (!vector_valid(vector))
        return VECTOR_ERROR_INIT;

    printf("[");
    for (size_t i = 0; i < vector->size; i++) {
        printf("%.6f", vector->elements[i]);
        if (i < vector->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    return VECTOR_SUCCESS;
}
