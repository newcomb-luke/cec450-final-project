#include "Effectors.h"
#include <semLib.h>
#include <vxworks.h>

void Valve_init(Valve* this) {
    this->_state = VALVE_CLOSED;
    this->_dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}

ValveState Valve_read(Valve* this) {
    ValveState state;

    semTake(this->_dataMutex, WAIT_FOREVER);

    state = this->_state;

    semGive(this->_dataMutex);

    return state;
}

void Valve_write(Valve* this, ValveState newState) {
    semTake(this->_dataMutex, WAIT_FOREVER);

    this->_state = newState;

    semGive(this->_dataMutex);
}




void Heater_init(Heater* this) {
    this->_state = HEATER_OFF;
    this->_dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}

HeaterState Heater_read(Heater* this) {
    HeaterState state;

    semTake(this->_dataMutex, WAIT_FOREVER);

    state = this->_state;

    semGive(this->_dataMutex);

    return state;
}

void Heater_write(Heater* this, HeaterState newState) {
    semTake(this->_dataMutex, WAIT_FOREVER);

    this->_state = newState;

    semGive(this->_dataMutex);
}
