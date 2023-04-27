#pragma once

#include "EffectorsMonitor.h"
#include "SensorsUpdater.h"
#include "Effectors.h"
#include "PressureSim.h"
#include <taskLib.h>

/**
 * Calculates the next water heater state and outputs new values to the sensors
 */
typedef struct {
    // The effector monitor used for input
    EffectorsMonitor _effectorsMonitor;
    // The sensors updater used for output
    SensorsUpdater _sensorsUpdater;
    // The latest effector states
    EffectorsStates _latestEffectorStates;
    // The internal pressure simulation for this simulator
    PressureSim _pressureSim;
    // The current simulated water level in the tank in centimeters
    float _waterLevel;
    // The simulator task’s task ID, initialized after Simulator_run()
    TASK_ID _taskID;
} Simulator;

/**
 * Data: effectors - The effectors this simulation will read as input.
 *   sensors - The sensors this simulation will update as output.
 * Purpose: This method initializes a Simulator class and acts like a constructor.
 * Pre-Condition: Simulator is uninitialized.
 * Post-Condition: effectorsMonitor is initialized using effectors. sensorsUpdater is initialized using sensors. pressureSim is initialized. waterLevel is set to 0.0. taskID is NULL. Simulator is initialized. Simulator is not started.
 * Returns: none.
 */
void Simulator_init(Simulator* this, EffectorsPackage effectors, SensorsPackage sensors);

/**
 * Purpose: This method starts the simulator’s internal task.
 * Pre-Condition: Simulator is initialized. Simulator is not started.
 * Post-Condition: taskID is not NULL. Simulator is started.
 * Returns: none.
 */
void Simulator_start(Simulator* this);

struct timespec subtractTimespecs(struct timespec before, struct timespec after);
float timespecToSeconds(struct timespec spec);

float float_min(float a, float b);
float float_max(float a, float b);
