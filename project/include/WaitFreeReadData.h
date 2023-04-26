#include <semLib.h>
#include <stddef.h>
#include <vxworks.h>

/**
 * Superclasses: none 
 * Subclasses: none 
 * Collaboration Graphs: See figure 4 
 * Description: provides a wait-free interface for reading from, and a blocking interface for writing to a value stored in memory. 
 * Contracts: WFRD 1 - Exclusively access or modify data 
 * Private variables:  
 * SEM_ID dataReadySem - Binary semaphore used to signal if new data is readable 
 * SEM_ID   - Mutex used to protect the data pointed to in this structure 
 * void* data - A pointer to data that is stored in this structure 
 * size_t sizeOfData - The size in bytes of the data stored
 */


typedef struct {
    SEM_ID _dataReadySem;
    SEM_ID _dataMutex;
    void* _data;
    size_t _sizeOfData;

} WaitFreeReadData;


/**
 * Data: sizeOfData - The size in bytes of the data that will be stored.
 * Purpose: This method initializes a WaitFreeReadData class 
 * and acts like a constructor.
 * Pre-Condition: WaitFreeReadData is uninitialized.
 * Post-Condition: dataReadySem is initialized and initially empty.
 * dataMutex is initialized and initially unlocked. data points to a 
 * newly allocated area on the heap. WaitFreeReadData is initialized.
 * Returns: none.
 */
void WaitFreeReadData_init(WaitFreeReadData* this, size_t _sizeOfData);

/**
 * Data: buffer - The buffer in which new data
 * will be written if there were any 
 * Purpose: This method reads new data into the provided
 * buffer if there is any available 
 * Pre-Condition: WaitFreeReadData is initialized. buffer is not NULL. 
 * Post-Condition:  If new data was available (dataReadySem is full),
 * the memory pointed to by buffer will be updated with the memory
 * pointed to by data. If no new data is available, buffer will
 * be unchanged. dataReadySem is empty..
 * Returns: true - if new data was read, false otherwise
 */
bool WaitFreeReadData_read(WaitFreeReadData* this, void* buffer);

/**
 * Data: newData - The buffer from which the new data will be read.
 * Purpose: This method writes newData into the
 * internal buffer in a blocking manner 
 * Pre-Condition: WaitFreeReadData is initialized. newData is not NULL. 
 * Post-Condition: the memory at data is now set to the value of the memory
 * at newData. dataReadySem is full.
 * Returns: none.
 */
void WaitFreeReadData_write(WaitFreeReadData* this, const void* newData);
