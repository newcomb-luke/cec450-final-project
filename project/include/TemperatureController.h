#pragma once

#include "Effectors.h"

/**
 * Represents the possible states of a temperature controller
 */
typedef enum {
    TEMP_CONTROLLER_HEATING = 0,
    TEMP_CONTROLLER_COOLING = 1
} TemperatureControllerState;

/**
 * Attempts to maintain the temperature of the water heater within bounds
 */
typedef struct {
    // The current state of the temperature controller
    TemperatureControllerState _state;
} TemperatureController;

/**
 * Purpose: This method initializes TemperatureController and acts like a constructor.
 * Pre-Condition: TemperatureController is uninitialized.
 * Post-Condition: state is set to TEMP_CONTROLLER_COOLING. TemperatureController is initialized.
 * Returns: none.
 */
void TemperatureController_init(TemperatureController* this);

/**
 * Data: temperature - The temperature of the water in the tank in degrees Celsius
 *   heaterState - A pointer to the output of this method: the new heater state.
 * Purpose: This method updates the controller and gets the new output.
 * Pre-Condition: TemperatureController is initialized.
 * Post-Condition: heaterState is updated if a change is required.
 * Returns: none.
 */
void TemperatureController_update(TemperatureController* this, float temperature, HeaterState* heaterState);
