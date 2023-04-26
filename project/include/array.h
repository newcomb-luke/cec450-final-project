#pragma once
#include <stddef.h>

/**
 * Description: provides access to a contiguous block of memory and provides
 * access to multiple items of the same type inside of it. Does not support
 * resizing. This is designed to only ever be created by vector_to_array_()
 */
typedef struct {
    //The number of items in this array
    size_t _size;
    //The size of an item in this array in bytes
    size_t _sizeOfItem;
    //The pointer to the start of the allocation for the array
    void* _start;
} array;

/**
 * Purpose: This method returns the size of the array in items.
 * Pre-Condition: array is initialized.
 * Post-Condition: none.
 * Returns: size_t - the number of items contained in the array.
 */
size_t array_size(array* this);

/**
 * Data: index - The index of the item to get.
 * Purpose: This method attempts to fetch an item from the index specified.
 * Pre-Condition: array is initialized.
 * Post-Condition: none.
 * Returns: if the index was valid, the return value is a non-NULL pointer to the value, if the index was invalid, the return value is NULL.
 */
void* array_get(array* this, size_t index);
