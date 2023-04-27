#pragma once

#include "Effectors.h"

/**
 * Calculates the pressure value to be used for the next iteration of the simulation
 */
typedef struct {
    // Stores the time-dependent calculated pressure from temperature going up
    float _pressureFromTemperature;
} PressureSim;

/**
 * Purpose: This method initializes a PressureSim class and acts like a constructor.
 * Pre-Condition: PressureSim is uninitialized.
 * Post-Condition: pressureFromTemperature is 0.0. PressureSim is initialized.
 * Returns: none.
 */
void PressureSim_init(PressureSim* this);

/**
 * Data: outletValveState - The state of the outlet valve.
 *   temperature - The tank water temperature.
 *   waterLevel - The water level in the tank in centimeters.
 *   deltaTime - The time since the last simulation update in seconds.
 * Purpose: This method updates the simulation of the pressure in the tank.
 * Pre-Condition: PressureSim is initialized.
 * Post-Condition: pressureFromTemperature is updated to a new value.
 * Returns: float - The total simulated pressure in the system.
 */
float PressureSim_update(PressureSim* this, ValveState outletValveState, float temperature, float waterLevel, float deltaTime);
