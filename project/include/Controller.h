#pragma once

#include "EffectorsUpdater.h"
#include "SensorsMonitor.h"
#include "SensorsUpdater.h"
#include "TemperatureController.h"
#include "Visualizer.h"
#include "WaterFlowController.h"

/**
 * Attempts to maintain the water heater within the specified requirements
 */
typedef struct {
    // The sensors monitor used for input
    SensorsMonitor _sensorsMonitor;
    // The effectors monitor used for output
    EffectorsUpdater _effectorsUpdater;
    // The controller system for temperature
    TemperatureController _temperatureController;
    // The controller system for valve commands and water flow
    WaterFlowController _waterFlowController;
    // A pointer to the system’s Visualizer for sending messages
    Visualizer* _visualizer;
    // The controller task’s task ID
    TASK_ID _taskID;
} Controller;

/**
 * Stores a collection of values that the Controller needs to be initialized
 */
typedef struct {
    ReadersPackage sensors;
    EffectorsPackage effectors;
    Visualizer* visualizer;
} ControllerInputs;

/**
 * Data: inputs - The input structures to the controller to allow it to function.
 * Purpose: This method initializes a Controller class and acts like a constructor.
 * Pre-Condition: Controller is uninitialized.
 * Post-Condition: sensorsMonitor is initialized using the inputs. effectorsUpdater is initialized using the inputs. temperatureController is initialized. waterFlowController is initialized. visualizer is set using inputs. taskID is NULL. Controller is initialized. Controller is not started.
 * Returns: none.
 */
void Controller_init(Controller* this, ControllerInputs inputs);

/**
 * Purpose: This method starts the controller’s internal task.
 * Pre-Condition: Controller is initialized. Controller is not started.
 * Post-Condition: taskID is not NULL. Controller is started.
 * Returns: none.
 */
void Controller_start(Controller* this);
