#pragma once

#include <semLib.h>
#include <stdbool.h>

/**
 * Allows exclusive access and modification to a water sensor state
 */
typedef struct {
    // Mutex used to protect the data in this structure
    SEM_ID dataMutex;
    // The current state of the water level sensor, if it has been reached
    bool value;
    // The location of the sensor in the tank
    float sensorLocation;
} WaterLevelSensor;

/**
 * Data: locationInTank - The location of the sensor in the tank, in centimeters from the bottom
 * Purpose: This method initializes a WaterLevelSensor class and acts like a constructor.
 * Pre-Condition: WaterLevelSensor is uninitialized.
 * Post-Condition: dataMutex is initialized and initially unlocked. value is false. sensorLocation is set to locationInTank.
 * Returns: none.
 */
void WaterLevelSensor_init(WaterLevelSensor* this, float locationInTank);

/**
 * Purpose: This method reads the current value of the sensor.
 * Pre-Condition: WaterLevelSensor is initialized.
 * Post-Condition: none.
 * Returns: bool - the value of the sensor
 */
bool WaterLevelSensor_read(WaterLevelSensor* this);

/**
 * Data: waterLevel - The current water level in the tank
 * Purpose: This method sets a new value for the sensor indirectly, by providing the water level for the sensor to compare itself to.
 * Pre-Condition: WaterLevelSensor is initialized.
 * Post-Condition: If waterLevel is greater than or equal to the sensors sensorLocation, then value is true, false otherwise
 * Returns: none.
 */
void WaterLevelSensor_write(WaterLevelSensor* this, float waterLevel);

/**
 * Allows exclusive access and modification to a sensor state
 */
typedef struct {
    // Mutex used to protect the data in this structure
    SEM_ID dataMutex;
    // The current state of the sensor
    float value;
} Sensor;

/**
 * Data: initialValue - The initial value for the sensor value to be initialized to.
 * Purpose: This method initializes a Sensor class and acts like a constructor.
 * Pre-Condition: Sensor is uninitialized.
 * Post-Condition: dataMutex is initialized and initially unlocked. value is set to initialValue. Sensor is initialized.
 * Returns: none.
 */
void Sensor_init(Sensor* this, float initialValue);

/**
 * Purpose: This method reads the current value of the sensor.
 * Pre-Condition: Sensor is initialized.
 * Post-Condition: none.
 * Returns: float - the value of the sensor
 */
float Sensor_read(Sensor* this);

/**
 * Data: newValue - The new value for the sensor
 * Purpose: This method sets a new value for the sensor.
 * Pre-Condition: Sensor is initialized.
 * Post-Condition: value is set to newValue
 * Returns: none.
 */
void Sensor_write(Sensor* this, float newValue);
