#include "PressureSim.h"
#include "Effectors.h"
#include "Simulator.h"

#define SECOND_TO_HIGHEST_VOLUME          (30.0)
#define PRESSURE_SCALE_FACTOR             (6.0)

#define EVAPORATION_FACTOR                (2.0)
#define EVAPORATION_START_TEMP            (85.0)

#define OUTLET_VALVE_PRESSURE_RELIEF_RATE (2.0)

void calculateOutletValvePressureRelief(ValveState outletValveState, float* pressureFromVolume, float* pressureFromTemperature);
float calculatePressureFromVolume(float waterVolume);
float calculatePressureFromTemperature(float pressureFromTemperature, float deltaTime, float waterTemp);

void PressureSim_init(PressureSim* this) {
    this->_pressureFromTemperature = 0.0;
}

float PressureSim_update(PressureSim* this, ValveState outletValveState, float temperature, float waterLevel, float deltaTime) { 
   this->_pressureFromTemperature = calculatePressureFromTemperature(this->_pressureFromTemperature, deltaTime, temperature);

   float pressureFromVolume = calculatePressureFromVolume(waterLevel);

   calculateOutletValvePressureRelief(outletValveState, &pressureFromVolume, &(this->_pressureFromTemperature));

   float pressure = this->_pressureFromTemperature + pressureFromVolume;

   return pressure;
}

float calculatePressureFromVolume(float waterLevel){
    if (waterLevel > SECOND_TO_HIGHEST_VOLUME) {
        float emptyVolume = waterLevel - SECOND_TO_HIGHEST_VOLUME;
        return emptyVolume * PRESSURE_SCALE_FACTOR;
    }

    return 0.0;
}

float calculatePressureFromTemperature(float pressureFromTemperature, float deltaTime, float temperature) {
    float tempDiff = temperature - EVAPORATION_START_TEMP;
    float evaporationPressureDiff = tempDiff * EVAPORATION_FACTOR * deltaTime;
    float newPressureFromTemperature = pressureFromTemperature + evaporationPressureDiff;

    return float_max(newPressureFromTemperature, 0.0);
}

void calculateOutletValvePressureRelief(ValveState outletValveState, float* pressureFromVolume, float* pressureFromTemperature) {
    if (outletValveState == VALVE_OPEN) {
        *pressureFromTemperature = float_max(*pressureFromTemperature - 
                                            OUTLET_VALVE_PRESSURE_RELIEF_RATE, 0.0);

        *pressureFromVolume = float_max(*pressureFromVolume -
                             OUTLET_VALVE_PRESSURE_RELIEF_RATE, 0.0);
    }
}
