#pragma once

#include "SensorValues.h"
#include "Sensors.h"
#include "array.h"
#include "WaitFreeReadData.h"

#include <wdLib.h>
#include <taskLib.h>

/**
 * Reads a collection of water level sensors and provides the most current aggregated water level reading
 */
typedef struct {
    // Data structure used to store state
    WaitFreeReadData _inner;
    // Stores water level sensors to be queried
    // array<WaterLevelSensor*>
    array _levelSensors;
    // Stores the last reading from this reader
    WaterLevelState _lastReading;
    // Stores the task id of the reader’s internal task
    TASK_ID _readerTask;
} WaterLevelReader;

/**
 * Data: sensors - The list of water level sensors to read.
 * Purpose: This method initializes a WaterLevelReader class and acts like a constructor.
 * Pre-Condition: WaterLevelReader is uninitialized.
 * Post-Condition: sensors are initialized. lastReading is initialized to a default value. 
 *  inner is an initialized WaitFreeReadData. readerTask is uninitialized. WaterLevelReader is initialized.
 * Returns: none.
 */
void WaterLevelReader_init(WaterLevelReader* this, array sensors);

/**
 * Purpose: This method will start the reader task that will actually perform sensor reading.
 * Pre-Condition: WaterLevelReader is initialized. WaterLevelReader hasn't been started.
 * Post-Condition: readerTask is not NULL. All variables except readerTask are unchanged.
 * Returns: none.
 */
void WaterLevelReader_start(WaterLevelReader* this);

/**
 * Purpose: This method will read a new water level or provide the last read value.
 * Pre-Condition: WaterLevelReader is initialized. WaterLevelReader has been started.
 * Post-Condition: lastReading is set to the new value if one was present
 * Returns: WaterLevelState - the new reading, or the previous reading if no new data was available
 * Uses: WaitFreeReadData_read()
 */
WaterLevelState WaterLevelReader_read(WaterLevelReader* this);



/**
 * Reads a sensor and provides the most current reading
 */
typedef struct {
    // Data structure used to store state
    WaitFreeReadData _inner;
    // Stores sensor to be queried
    Sensor* _sensor;
    // Stores the last reading from this reader
    float _lastReading;
    // Stores the watchdog id of the reader’s internal watchdog
    WDOG_ID _readerWatchdog;
} SensorReader;

/**
 * Data: sensor - The sensor to read.
 *       initialValue - An initial value for the sensor
 * Purpose: This method initializes a SensorReader class and acts like a constructor.
 * Pre-Condition: SensorReader is uninitialized.
 * Post-Condition: sensor is initialized. lastReading is initialized to a default value. inner is an initialized WaitFreeReadData. readerWatchdog is uninitialized. SensorReader is initialized.
 * Returns: none.
 */
void SensorReader_init(SensorReader* this, Sensor* sensor, float initialValue);

/**
 * Purpose: This method will start the reader task that will actually perform sensor reading.
 * Pre-Condition: SensorReader is initialized. SensorReader hasn't been started.
 * Post-Condition: readerWatchdog is not NULL.
 * Returns: none.
 */
void SensorReader_start(SensorReader* this);

/**
 * Purpose: This method will read a new sensor reading or provide the last read value.
 * Pre-Condition: SensorReader is initialized. SensorReader has been started.
 * Post-Condition: lastReading is set to the new value if one was present
 * Returns: float - the new sensor reading, or the previous reading if no new data was available
 * Uses: WaitFreeReadData_read()
 */
float SensorReader_read(SensorReader* this);
