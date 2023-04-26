#include <semLib.h>
#include <stddef.h>
#include "WaitFreeReadData.h"
#include <vxworks.h>
#include <string.h>

void WaitFreeReadData_init(WaitFreeReadData* this, size_t sizeOfData) {
    this->_dataReadySem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

    this->_dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

    this->_sizeOfData = sizeOfData;
}


bool WaitFreeReadData_read(WaitFreeReadData *this, void *buffer) {
    // If readableSim is full
    if (semTake(this->_dataReadySem, NO_WAIT) == OK) {
        if (semTake(this->_dataMutex, NO_WAIT) == OK) {
            memcpy(buffer, this->_data, this->_sizeOfData);

            semGive(this->_dataMutex);

            return true;
        }
    }

    return false;
}

void WaitFreeReadData_write(WaitFreeReadData* this, const void* newData) {
    semTake(this->_dataMutex, WAIT_FOREVER);

    memcpy(this->_data, newData, NO_WAIT);

    semGive(this->_dataMutex);

    semGive(this->_dataReadySem);
}
