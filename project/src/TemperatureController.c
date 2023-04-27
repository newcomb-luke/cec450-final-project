#include "TemperatureController.h"
#include "Effectors.h"

#define CONTROLLER_UPPER_TEMP_BOUND (80.0)
#define CONTROLLER_LOWER_TEMP_BOUND (30.0)

void TemperatureController_init(TemperatureController* this) {
    this->_state = TEMP_CONTROLLER_COOLING;
}

void TemperatureController_update(TemperatureController* this, float temperature, HeaterState* heaterState) {
    if (this->_state == TEMP_CONTROLLER_COOLING) {
        if (temperature <= CONTROLLER_LOWER_TEMP_BOUND) {
            *heaterState = HEATER_ON;
            this->_state = TEMP_CONTROLLER_HEATING;
        }
    } else {
        if (temperature >= CONTROLLER_UPPER_TEMP_BOUND) {
            *heaterState = HEATER_OFF;
            this->_state = TEMP_CONTROLLER_COOLING;
        }
    }
}
