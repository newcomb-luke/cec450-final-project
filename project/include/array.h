#pragma once
#include <stddef.h>

/**
 * Description: provides access to a contiguous block of memory and provides
 * access to multiple items of the same type inside of it. Does not support 
 * resizing
 */
typedef struct {
    const size_t _size;
    const size_t _sizeOfItem;
    void* const _start;
} array;

/**
 * size_t array_size()
 * Purpose: This method returns the size of the array in items.
 * Pre-Condition: array is initialized.
 * Post-Condition: none.
 * Returns: size_t - the number of items contained in the array.
 */

size_t array_size(array* this);

/**
 * void* array_get(size_t index)
 * Data: index - The index of the item to get.
 * Purpose: This method attempts to fetch an item from the index specified.
 * Pre-Condition: array is initialized.
 * Post-Condition: none.
 * Returns: if the index was valid, the return value is a non-NULL pointer to the value, if the index was invalid, the return value is NULL.
 */
void* array_get(array* this, size_t index);
