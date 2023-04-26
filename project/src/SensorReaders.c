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
    this->levelSensors = sensors;

    this->lastReading = WATER_LEVEL_EMPTY;

    this->readerTask = NULL;

    WaitFreeReadData_init(&this->inner, sizeof(WaterLevelState));
}

void WaterLevelReader_start(WaterLevelReader* this) {
    this->readerTask = taskSpawn("waterLevelReader",
                                 WATER_LEVEL_READER_PRIORITY, 
                                 0, 0x1000, (FUNCPTR) _WaterLevelReader_loop,
                                 (size_t) this,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

WaterLevelState WaterLevelReader_read(WaterLevelReader* this) {
    WaterLevelState newReading;

    if (WaitFreeReadData_read(&this->inner, &newReading)) {
        this->lastReading = newReading;
    }

    return this->lastReading;
}

void _WaterLevelReader_loop(WaterLevelReader* this) {
    WaterLevelState last = this->lastReading;

    const int numSensors = array_size(&(this->levelSensors));

    // Infinite loop
    for (;;) {

        int hitCount;

        for (hitCount = 0; hitCount < numSensors; hitCount++) {
            WaterLevelSensor* sensor = *((WaterLevelSensor**) array_get(&(this->levelSensors), hitCount));

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
            WaitFreeReadData_write(&(this->inner), &newWaterLevel);

            last = newWaterLevel;
        }

        taskDelay(WATER_LEVEL_READER_DELAY);
    }
}

void SensorReader_init(SensorReader* this, Sensor* sensor, float initialValue) {
    this->sensor = sensor;

    this->lastReading = initialValue;

    this->readerWatchdog = NULL;

    WaitFreeReadData_init(&this->inner, sizeof(float));
}

void SensorReader_start(SensorReader* this) {
    this->readerWatchdog = wdCreate();
    wdStart(this->readerWatchdog, 
            SENSOR_READER_ACTIVATION_DELAY, 
            (FUNCPTR) _SensorReader_loop,
            (int) this);
}

float SensorReader_read(SensorReader* this) {
    float newReading;

    if (WaitFreeReadData_read(&this->inner, &newReading)) {
        this->lastReading = newReading;
    }

    return this->lastReading;
}

void _SensorReader_loop(SensorReader* this) {
    wdStart(this->readerWatchdog, 
            SENSOR_READER_ACTIVATION_DELAY, 
            (FUNCPTR) _SensorReader_loop,
            (int) this);

    float newReading = Sensor_read(this->sensor);

    if (newReading != this->lastReading) {
        WaitFreeReadData_write(&(this->inner), &newReading);

        this->lastReading = newReading;
    }
}
