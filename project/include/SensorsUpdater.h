#pragma once

#include "array.h"
#include "Sensors.h"

/**
 * Stores a collection of values to update the sensors with using the SensorsUpdater
 */
typedef struct {
    float waterLevel;
    float pressure;
    float temperature;
} SensorUpdates;

/**
 * Stores a collection of sensors to be provided to a SensorsUpdater
 */
typedef struct {
    array waterLevelSensors;
    Sensor* pressureSensor;
    Sensor* temperatureSensor;
} SensorsPackage;

/**
 * Provides an interface for the simulation to update sensor values
 */
typedef struct {
    // Stores the sensors that this should update
    SensorsPackage _sensors;
    // The previous sensor readings, used to compare to the new ones 
    // in order to prevent duplicate data from being written
    SensorUpdates _previousValues;
} SensorsUpdater;

/**
 * Data: sensorsToUpdate - The collection of sensors that this updater is responsible for.
 * Purpose: This method initializes a SensorsUpdater class and acts like a constructor.
 * Pre-Condition: SensorsUpdater is uninitialized.
 * Post-Condition: sensors is initialized to sensorsToUpdate, and previousValues is set to all -1.0 (impossible under any real conditions). SensorsUpdater is initialized.
 * Returns: none.
 */
void SensorsUpdater_init(SensorsUpdater* this, SensorsPackage sensorsToUpdate);

/**
 * Data: updates - The new set of sensor readings to write to each sensor.
 * Purpose: This method updates the values for each of the sensors in the system.
 * Pre-Condition: SensorsUpdater is initialized.
 * Post-Condition: previousValues is set to the values of updates. If sensor readings changed, then they are updated.
 * Returns: none.
 */
void SensorsUpdater_write(SensorsUpdater* this, SensorUpdates updates);
