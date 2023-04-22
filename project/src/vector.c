#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "string.h"
#include "array.h"

#define INITIAL_CAPACITY 2




void vector_init(vector* this, size_t sizeOfItem) {
    this->_sizeOfItem = sizeOfItem;
    this->_size = 0;
    this->_capacity = INITIAL_CAPACITY;
    this->_start = malloc(this->_capacity * sizeOfItem);
}



size_t vector_size(vector* this) {
    return this->_size;
}



void vector_push(vector* this, void* itemPtr) {

    if (this->_size >= this->_capacity) {
        this->_capacity *= 2;

        this->_start = realloc(this->_start, this->_capacity * this->_sizeOfItem);
    }

    void* nextItemPtr = this->_start + this->_size * this->_sizeOfItem;

    memcpy(nextItemPtr, itemPtr, this->_sizeOfItem);

    this->_size += 1;
}



void* vector_get(vector* this, size_t index) {
    if (index <= this->_size && index >= 0) {
        return (this->_start + (index * this->_sizeOfItem));
    } else {
        return NULL;
    }
}



array vector_to_array(vector* this) {
    array new_array = {
       ._size = this->_size,
       ._sizeOfItem = this->_sizeOfItem,
       ._start = this->_start
    };

    return new_array;
}
