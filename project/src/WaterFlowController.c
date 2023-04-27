#include "WaterFlowController.h"
#include "Effectors.h"
#include "SensorValues.h"

#define WATER_LEVEL_LOWER_BOUND (10.0)
#define WATER_LEVEL_UPPER_BOUND (40.0)
#define PRESSURE_UPPER_BOUND    (320.0)

void WaterFlowController_init(WaterFlowController* this) {
    this->_state = FLOW_CONTROLLER_EMPTYING;
}

void WaterFlowController_update(WaterFlowController* this, float pressure, WaterLevelState waterLevel, ValveStates* valveStates) {
    float waterLevelFloat = 0.0;

    if (waterLevel == WATER_LEVEL_EMPTY) {
        waterLevelFloat = 5.0;
    } else if (waterLevel == WATER_LEVEL_LOW) {
        waterLevelFloat = 15.0;
    } else if (waterLevel == WATER_LEVEL_MID_LOW) {
        waterLevelFloat = 25.0;
    } else if (waterLevel == WATER_LEVEL_MID_HIGH) {
        waterLevelFloat = 35.0;
    } else if (waterLevel == WATER_LEVEL_HIGH) {
        waterLevelFloat = 45.0;
    }

    if (this->_state == FLOW_CONTROLLER_EMPTYING) {
        if (waterLevelFloat <= WATER_LEVEL_LOWER_BOUND) {
            valveStates->outletValveState = VALVE_CLOSED;

            const int numInletValves = array_size(&(valveStates->inletValveStates));
            int i;
            for (i = 0; i < numInletValves; i++) {
                ValveState* state = array_get(&(valveStates->inletValveStates), i);

                *state = VALVE_OPEN;
            }
            
            this->_state = FLOW_CONTROLLER_FILLING;
        }
    } else {
        if (waterLevelFloat >= WATER_LEVEL_UPPER_BOUND || pressure >= PRESSURE_UPPER_BOUND) {
            valveStates->outletValveState = VALVE_OPEN;

            const int numInletValves = array_size(&(valveStates->inletValveStates));
            int i;
            for (i = 0; i < numInletValves; i++) {
                ValveState* state = array_get(&(valveStates->inletValveStates), i);

                *state = VALVE_CLOSED;
            }
            
            this->_state = FLOW_CONTROLLER_EMPTYING;
        }
    }
}
