#pragma once

/**
 * Represents the possible states of a water flow controller
 */
#include "Effectors.h"
#include "SensorValues.h"
typedef enum {
    FLOW_CONTROLLER_FILLING = 0,
    FLOW_CONTROLLER_EMPTYING = 1
} WaterFlowControllerState;

/**
 * Attempts to maintain the flow of water and pressure of the water heater within bounds
 */
typedef struct {
    // The current state of the water flow controller
    WaterFlowControllerState _state;
} WaterFlowController;

/**
 * Purpose: This method initializes a WaterFlowController class and acts like a constructor.
 * Pre-Condition: WaterFlowController is uninitialized.
 * Post-Condition: state is set to FLOW_CONTROLLER_FILLING. WaterFlowController is initialized.
 * Returns: none.
 */
void WaterFlowController_init(WaterFlowController* this);

/**
 * Data: pressure - The pressure of the tank in kilopascals.
 *   waterLevel - The current water level in the tank.
 *   valveStates - A pointer to the output of this method: system valve states.
 * Purpose: This method updates the controller and gets the new output.
 * Pre-Condition: WaterFlowController is initialized.
 * Post-Condition: valveStates is updated if a change is required.
 * Returns: none.
 */
void WaterFlowController_update(WaterFlowController* this, float pressure, WaterLevelState waterLevel, ValveStates* valveStates);
