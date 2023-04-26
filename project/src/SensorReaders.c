#include "SensorReaders.h"
#include "SensorValues.h"
#include "Sensors.h"

#include <wdLib.h>
#include <taskLib.h>
#include <vxworks.h>

#define WATER_LEVEL_READER_PRIORITY    (100)
#define WATER_LEVEL_READER_DELAY       (10)

#define SENSOR_READER_ACTIVATION_DELAY (10)

void _WaterLevelReader_loop(WaterLevelReader* this);
void _SensorReader_loop(SensorReader* this);

void WaterLevelReader_init(WaterLevelReader* this, array sensors) {
    this->_levelSensors = sensors;

    this->_lastReading = WATER_LEVEL_EMPTY;

    this->_readerTask = NULL;

    WaitFreeReadData_init(&this->_inner, sizeof(WaterLevelState));
}

void WaterLevelReader_start(WaterLevelReader* this) {
    this->_readerTask = taskSpawn("waterLevelReader",
                                 WATER_LEVEL_READER_PRIORITY, 
                                 0, 0x2000, (FUNCPTR) _WaterLevelReader_loop,
                                 (size_t) this,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

WaterLevelState WaterLevelReader_read(WaterLevelReader* this) {
    WaterLevelState newReading;

    if (WaitFreeReadData_read(&this->_inner, &newReading)) {
        this->_lastReading = newReading;
    }

    return this->_lastReading;
}

void _WaterLevelReader_loop(WaterLevelReader* this) {
    WaterLevelState last = this->_lastReading;

    const int numSensors = array_size(&(this->_levelSensors));

    // Infinite loop
    for (;;) {
        int hitCount;

        for (hitCount = 0; hitCount < numSensors; hitCount++) {
            WaterLevelSensor* sensor = array_get(&(this->_levelSensors), hitCount);

            // If the sensor is not reached, break
            if (!WaterLevelSensor_read(sensor)) {
                break;
            }
        }

        // Sensible default fault-wise
        WaterLevelState newWaterLevel = WATER_LEVEL_EMPTY;

        switch (hitCount) {
            case 0:
                newWaterLevel = WATER_LEVEL_EMPTY;
                break;
            case 1:
                newWaterLevel = WATER_LEVEL_LOW;
                break;
            case 2:
                newWaterLevel = WATER_LEVEL_MID_LOW;
                break;
            case 3:
                newWaterLevel = WATER_LEVEL_MID_HIGH;
                break;
            case 4:
                newWaterLevel = WATER_LEVEL_HIGH;
                break;
        }

        if (newWaterLevel != last) {
            WaitFreeReadData_write(&(this->_inner), &newWaterLevel);

            last = newWaterLevel;
        }

        taskDelay(WATER_LEVEL_READER_DELAY);
    }
}

void SensorReader_init(SensorReader* this, Sensor* sensor, float initialValue) {
    this->_sensor = sensor;

    this->_lastReading = initialValue;

    this->_readerWatchdog = NULL;

    WaitFreeReadData_init(&this->_inner, sizeof(float));
}

void SensorReader_start(SensorReader* this) {
    this->_readerWatchdog = wdCreate();

    int status = wdStart(this->_readerWatchdog, 
            SENSOR_READER_ACTIVATION_DELAY, 
            (FUNCPTR) _SensorReader_loop,
            (size_t) this);
}

float SensorReader_read(SensorReader* this) {
    float newReading;

    if (WaitFreeReadData_read(&this->_inner, &newReading)) {
        printf("Got new reading: %f\n", newReading);
        this->_lastReading = newReading;
    }

    return this->_lastReading;
}

void _SensorReader_loop(SensorReader* this) {
    wdStart(this->_readerWatchdog, 
            SENSOR_READER_ACTIVATION_DELAY, 
            (FUNCPTR) _SensorReader_loop,
            (size_t) this);

    float newReading = Sensor_read(this->_sensor);

    if (newReading != this->_lastReading) {
        WaitFreeReadData_write(&(this->_inner), &newReading);

        this->_lastReading = newReading;
    }
}
