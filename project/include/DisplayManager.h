#pragma once

#include "Effectors.h"
#include "SensorValues.h"

/**
 * Maintains the state of, updates, and draws the current state of the visualization
 */
typedef struct {
    // The internal buffer that all updates operate on
    char* _displayBuffer;
} DisplayManager;

/**
 * Purpose: This method initializes a DisplayManager class and acts like a constructor.
 * Pre-Condition: DisplayManager is uninitialized.
 * Post-Condition: displayBuffer is not NULL and initialized with an initial image. DisplayManager is initialized.
 * Returns: none.
 */
void DisplayManager_init(DisplayManager* this);

/**
 * Purpose: This method redraws the display using the current contents of the displayBuffer.
 * Pre-Condition: DisplayManager is initialized.
 * Post-Condition: The displayBuffer is written to the screen.
 * Returns: none.
 */
void DisplayManager_draw(const DisplayManager* this);

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
