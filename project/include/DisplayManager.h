#pragma once

#include "Effectors.h"
#include "SensorValues.h"

/**
 * Maintains the state of, updates, and draws the current state of the visualization
 */
typedef struct {
    // The internal buffer that all updates operate on
    char* _displayBuffer;
    // The index of the next line to write a log message to
    int _nextLogMessageLine;
} DisplayManager;

/**
 * Purpose: This method initializes a DisplayManager class and acts like a constructor.
 * Pre-Condition: DisplayManager is uninitialized.
 * Post-Condition: displayBuffer is not NULL and initialized with an initial image. DisplayManager is initialized.
 * Returns: none.
 */
void DisplayManager_init(DisplayManager* this);

/**
 * Data: state - The new state to set the inlet valve indicator.
 * Purpose: This method sets the state of the inlet valve 1 indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setInletValve1(DisplayManager* this, ValveState state);

/**
 * Data: state - The new state to set the inlet valve indicator.
 * Purpose: This method sets the state of the inlet valve 2 indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setInletValve2(DisplayManager* this, ValveState state);

/**
 * Data: state - The new state to set the outlet valve indicator.
 * Purpose: This method sets the state of the outlet valve indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setOutletValve(DisplayManager* this, ValveState state);

/**
 * Data: state - The new state to set the heater indicator.
 * Purpose: This method sets the state of the heater indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setHeater(DisplayManager* this, HeaterState state);

/**
 * Data: waterLevel - The new water level.
 * Purpose: This method sets the state of the water level indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setWaterLevel(DisplayManager* this, WaterLevelState state);

/**
 * Data: temperature - The new temperature in degrees celsius.
 * Purpose: This method sets the state of the temperature indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setTemperature(DisplayManager* this, float temperature);

/**
 * Data: pressure - The new pressure in kilopascals.
 * Purpose: This method sets the state of the pressure indicator.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer may be updated.
 * Returns: none.
 */
void DisplayManager_setPressure(DisplayManager* this, float pressure);

/**
 * Data: msg - The new log message string.
 * Purpose: This method adds a new log message to the section of the display buffer.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The internal displayBuffer is updated.
 * Returns: none.
 */
void DisplayManager_logMessage(DisplayManager* this, const char* msg);

/**
 * Purpose: This method redraws the display using the current contents of the displayBuffer.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The displayBuffer is written to the screen.
 * Returns: none.
 */
void DisplayManager_draw(const DisplayManager* this);
