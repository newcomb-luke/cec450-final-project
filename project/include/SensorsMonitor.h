#pragma once

#include "SensorReaders.h"
#include "SensorValues.h"
#include "array.h"

/**
 * Stores a collection of sensor readers to give to the SensorsMonitor
 */
typedef struct {
    WaterLevelReader* waterLevelReader;
    SensorReader* pressureReader;
    SensorReader* temperatureReader;
} ReadersPackage;

/**
 * Provides an interface for the controller to read sensor values
 */
typedef struct {
    // Stores the sensors that this should update
    ReadersPackage _readers;
} SensorsMonitor;

/**
 * Data: readersToMonitor - The collection of sensor readers to be read from.
 * Purpose: This method initializes a SensorsMonitor class and acts like a constructor.
 * Pre-Condition: SensorsMonitor is uninitialized.
 * Post-Condition: readers is initialized to the value of readersToMonitor. SensorsMonitor is initialized.
 * Returns: none.
 */
void SensorsMonitor_init(SensorsMonitor* this, ReadersPackage readersToMonitor);

/**
 * Purpose: This method starts the sensor readers that this is responsible for.
 * Pre-Condition: SensorsMonitor is initialized.
 * Post-Condition: SensorsMonitor is started. All SensorReaders are started.
 * Returns: none.
 */
void SensorsMonitor_start(SensorsMonitor* this);

/**
 * Purpose: This method reads the values for each of the sensors in the system.
 * Pre-Condition: SensorsMonitor is initialized. SensorsMonitor is started.
 * Post-Condition: All readers in the readers variable have been read from, and all dataReadySem semaphores are empty.
 * Returns: SensorReadings - The most current readings from all of the sensor readers.
 */
SensorReadings SensorsMonitor_read(SensorsMonitor* this);
