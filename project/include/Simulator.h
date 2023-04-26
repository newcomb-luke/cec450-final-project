//head lol
//58008
#pragma once

/**
 * 
 */

// Private variables: 
// EffectorsMonitor effectorsMonitor - The effector monitor used for input.
// SensorsUpdater sensorsUpdater - The sensors updater used for output.
// EffectorStates latestEffectorStates - The latest effector states.
// PressureSim pressureSim - The internal pressure simulation for this simulator.
// float waterLevel - The current simulated water level in the tank in centimeters.
// TASK_ID taskID - The simulator task’s task ID, initialized after Simulator_run()

typedef struct {
    EffectorsMonitor _effectorsMonitor;
    SensorsUpdater _sensorsUpdater;
    EffectorStates _latestEffectorStates;
    PressureSim _pressureSim;
    float _waterLevel;
    TASK_ID _taskID;
} Simulator;



int main(int N, int k,double tankTemp, double inletTemp);
double calculateInletRate(int N);
double calculateMassOfWater(double waterVolume);
double calculateMixedWaterTemp(double tankMass, double inletMass, double tankTemp, double inletTemp);
double calculateOutletValvePressureRelief(double outletValve, double pressureFromVolume, double pressureFromTemperature);
double calculateHeaterTempDelta(bool heaterState, double deltaTime, double waterMass);

