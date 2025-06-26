/**
 * @file vector.c
 * @brief Vector computation
 * @date 25/06/25
 */

#include "vector.h"
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
