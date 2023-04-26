// Class: PressureSim
// Superclasses: none
// Subclasses: none
// Collaboration Graphs: See figure 6
// Description: calculates the pressure value to be used for the next iteration of the simulation
// Contracts: PS 1 - Update pressure simulation
// Private variables: 
// float pressureFromTemperature - Stores the time-dependent calculated pressure from temperature going up.
#include <PressureSim.h>
#include <Simulator.h>
double calculatePressureFromVolume(double waterVolume);
double calculatePressureFromTemperature(double pressureFromTempurature, double deltaTime, double waterTemp);
// void PressureSim_init()
// Purpose: This method initializes a PressureSim class and acts like a constructor.
// Pre-Condition: PressureSim is uninitialized.
// Post-Condition: pressureFromTemperature is 0.0. PressureSim is initialized.
// Returns: none.

void PressureSim_init(PressureSim* this){
    this->_pressureFromTemperature =0.0;
}

// float PressureSim_update(ValveState outletValveState, float temperature, float waterLevel, float deltaTime)
// Data: outletValveState - The state of the outlet valve.
// temperature - The tank water temperature.
// waterLevel - The water level in the tank in centimeters.
// deltaTime - The time since the last simulation update in seconds.
// Purpose: This method updates the simulation of the pressure in the tank.
// Pre-Condition: PressureSim is initialized.
// Post-Condition: pressureFromTemperature is updated to a new value.
// Returns: float - The total simulated pressure in the system.

float PressureSim_update(PressureSim* this,ValveState outletValveState, float temperature, float waterLevel, float deltaTime){ 
   this->_pressureFromTemperature += calculatePressureFromTemperature(this->_pressureFromTemperature, deltaTime, temperature);
   return this->_pressureFromTemperature + calculatePressureFromVolume(waterLevel);
}

double calculatePressureFromVolume(double waterLevel){

int SECOND_TO_HIGHEST_VOLUME = 30;
int SCALE_FACTOR = 6;

if (waterVolume > SECOND_TO_HIGHEST_VOLUME){
	double emptyVolume = waterVolume - SECOND_TO_HIGHEST_VOLUME;
	return emptyVolume * SCALE_FACTOR;
}

return 0.0;
}

double calculatePressureFromTemperature(double pressureFromTempurature, double deltaTime, double temperature){

    int EVAPORATION_FACTOR = 2;
    int EVAPORATION_START_TEMP = 85;

    double tempDiff = temperature - EVAPORATION_START_TEMP;
    double evaporationPressureDiff = tempDiff * EVAPORATION_FACTOR * deltaTime;
    double pressureFromTemperature = pressureFromTemperature + evaporationPressureDiff;

    return max(pressureFromTemperature, 0);
}

float calculateOutletValvePressureRelief(double outletValve, double pressureFromVolume, double pressureFromTemperature){

    int OUTLET_VALVE_PRESSURE_RELIEF_RATE = 2;

    if (outletValve.isOpen() ){
        pressureFromTemperature = max(pressureFromTemperature - 
    OUTLET_VALVE_PRESSURE_RELIEF_RATE, 0);
        pressureFromVolume = max(pressureFromVolume -
                        OUTLET_VALVE_PRESSURE_RELIEF_RATE, 0);
    }

    return (pressureFromVolume, pressureFromTemperature);
}
