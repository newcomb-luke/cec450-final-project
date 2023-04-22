#include "array.h"
#include <stddef.h>

size_t array_size(array* this) {
  return this->_size;  
}

void* array_get(array* this, size_t index) {
   if (index <= this->_size && index >= 0) {
       return (this->_start + (index * this->_sizeOfItem));
   } else {
       return NULL;
   }
}
