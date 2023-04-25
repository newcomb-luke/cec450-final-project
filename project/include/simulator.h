//head lol
//58008
#pragma once

// Private variables: 
// EffectorsMonitor effectorsMonitor - The effector monitor used for input.
// SensorsUpdater sensorsUpdater - The sensors updater used for output.
// EffectorStates latestEffectorStates - The latest effector states.
// PressureSim pressureSim - The internal pressure simulation for this simulator.
// float waterLevel - The current simulated water level in the tank in centimeters.
// TASK_ID taskID - The simulator task’s task ID, initialized after Simulator_run()

// void Simulator_init(EffectorsPackage effectors, SensorsPackage sensors)
// Data: effectors - The effectors this simulation will read as input.
// sensors - The sensors this simulation will update as output.
// Purpose: This method initializes a Simulator class and acts like a constructor.
// Pre-Condition: Simulator is uninitialized.
// Post-Condition: effectorsMonitor is initialized using effectors. sensorsUpdater is initialized using sensors. pressureSim is initialized. waterLevel is set to 0.0. taskID is NULL. Simulator is initialized. Simulator is not started.
// Returns: none.

// void Simulator_start()
// Purpose: This method starts the simulator’s internal task.
// Pre-Condition: Simulator is initialized. Simulator is not started.
// Post-Condition: taskID is not NULL. Simulator is started.
// Returns: none.


// void _Simulator_loop()
// Purpose: This method is the Simulator’s main loop that is the entry point of a task.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: never.

// float _Simulator_calculateInletRate()
// Purpose: This method calculates the inlet rate of water into the tank using valve states.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: float - the inlet rate in Liters per second.

// float _Simulator_calculateMixedWaterTemp(float tankTemperature, float inletMass)
// Data: tankTemperature - The temperature of the water in the tank in degrees Celsius
// inletMass - The mass of the current amount of water that is entering the tank.
// Purpose: This method calculates the final temperature of mixing inlet water into the tank.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: float - the final temperature after mixing.

// float _Simulator_calculateWaterHeating()
// Purpose: This method calculates the change in water temperature for this next update.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: float - the change in water temperature due to the heater.