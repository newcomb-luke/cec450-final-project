#include "Sensors.h"
#include <semLib.h>
#include <vxworks.h>

void WaterLevelSensor_init(WaterLevelSensor* this, float locationInTank) {
    this->dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
    this->sensorLocation = locationInTank;
    this->value = false;
}

bool WaterLevelSensor_read(WaterLevelSensor* this) {
    bool value;

    semTake(this->dataMutex, WAIT_FOREVER);

    value = this->value;

    semGive(this->dataMutex);

    return value;
}

void WaterLevelSensor_write(WaterLevelSensor* this, float waterLevel) {
    semTake(this->dataMutex, WAIT_FOREVER);

    this->value = waterLevel >= this->sensorLocation;

    semGive(this->dataMutex);
}




void Sensor_init(Sensor* this, float initialValue) {
    this->value = initialValue;
    this->dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}

float Sensor_read(Sensor* this) {
    float value;

    semTake(this->dataMutex, WAIT_FOREVER);

    value = this->value;

    semGive(this->dataMutex);

    return value;
}

void Sensor_write(Sensor* this, float newValue) {
    semTake(this->dataMutex, WAIT_FOREVER);

    this->value = newValue;

    semGive(this->dataMutex);
}
