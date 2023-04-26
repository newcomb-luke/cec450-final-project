#include "Sensors.h"
#include <semLib.h>
#include <vxworks.h>

void WaterLevelSensor_init(WaterLevelSensor* this, float locationInTank) {
    this->_dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
    this->_sensorLocation = locationInTank;
    this->_value = false;
}

bool WaterLevelSensor_read(WaterLevelSensor* this) {
    bool value;

    semTake(this->_dataMutex, WAIT_FOREVER);

    value = this->_value;

    semGive(this->_dataMutex);

    return value;
}

void WaterLevelSensor_write(WaterLevelSensor* this, float waterLevel) {
    semTake(this->_dataMutex, WAIT_FOREVER);

    this->_value = waterLevel >= this->_sensorLocation;

    semGive(this->_dataMutex);
}




void Sensor_init(Sensor* this, float initialValue) {
    this->_value = initialValue;
    this->_dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}

float Sensor_read(Sensor* this) {
    float value;

    semTake(this->_dataMutex, WAIT_FOREVER);

    value = this->_value;

    semGive(this->_dataMutex);

    return value;
}

void Sensor_write(Sensor* this, float newValue) {
    semTake(this->_dataMutex, WAIT_FOREVER);

    this->_value = newValue;

    semGive(this->_dataMutex);
}
