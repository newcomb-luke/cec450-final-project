#include "SensorsUpdater.h"

void SensorsUpdater_init(SensorsUpdater* this, SensorsPackage sensorsToUpdate) {
    this->_sensors = sensorsToUpdate;
    this->_previousValues.waterLevel = -1.0;
    this->_previousValues.temperature = -1.0;
    this->_previousValues.pressure = -1.0;
}

void SensorsUpdater_write(SensorsUpdater* this, SensorUpdates updates) {
    // Water level sensors
    if (this->_previousValues.waterLevel != updates.waterLevel) {
        int i;
        for (i = 0; i < array_size(&this->_sensors.waterLevelSensors); i++) {
            WaterLevelSensor* sensor = array_get(&this->_sensors.waterLevelSensors, i);
            WaterLevelSensor_write(sensor, updates.waterLevel);
        }
    }

    // Temperature sensor
    if (this->_previousValues.temperature != updates.temperature) {
        Sensor_write(this->_sensors.temperatureSensor, updates.temperature);
    }

    // Pressure sensor
    if (this->_previousValues.pressure != updates.pressure) {
        Sensor_write(this->_sensors.pressureSensor, updates.pressure);
    }
}
