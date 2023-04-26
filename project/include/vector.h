#pragma once
#include <stddef.h>
#include "array.h"
/**
 * provides access to a contiguous block of memory and 
 * provides access to multiple items of the same type inside of it. 
 * Does not support resizing. This is designed to only ever
 * be created by vector_to_array_()
 */
typedef struct {
    size_t _capacity;
    size_t _size;
    size_t _sizeOfItem;
    void* _start;
} vector;

/**
 * Data: sizeOfItem - The size of the items that will be stored in bytes.
 * Purpose: This method acts like a constructor for vector.
 * Pre-Condition: vector is not initialized.
 * Post-Condition: vector is initialized. _sizeOfItem is set to sizeOfItem. 
 *      start is not NULL. size is 0. capacity is set to a default non-zero value.
 * Returns: none.
*/
void vector_init(vector* this, size_t sizeOfItem);

/**
 * Purpose: This method returns the size of the vector in items.
 * Pre-Condition: vector is initialized.
 * Post-Condition: none.
 * Returns: size_t - the number of items contained in the vector.
 */
size_t vector_size();

/**
 * Data: itemPtr - The pointer to the data to be copied into the vector 
 * Purpose: This method appends an item to the back of the vector and resizes if necessary.
 * Pre-Condition: vector is initialized. There are no pointers to internal data in the vector.
 * Post-Condition: the item is stored in the back of the vector.
 * Returns: none.
 */
void vector_push(vector* this, void* itemPtr);

/**
 * Data: index - The index of the item to get
 * Purpose: This method attempts to fetch an item from the index specified.
 * Pre-Condition: vector is initialized.
 * Post-Condition: none.
 * Returns: if the index was valid, the return value is a non-NULL pointer
 *       to the value, if the index was invalid (less than 0 or greater than
 *       or equal to the number of items contained), the return value is NULL. 
 *       The pointers is only valid until another push occurs.
 */
void* vector_get(vector* this, size_t index);

/**
 * Purpose: This method effectively converts this vector to an array.
 * Pre-Condition: vector is initialized.
 * Post-Condition: vector must never be used again.
 * Returns: array - the converted array structure.
 */
array vector_to_array(vector* this);
