#include <semlib.h>

enum HeaterState {
    Off = 0,
    On = 1
};

void Heater_init(Heater* this) {
    this->heaterState = off;
    this->dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}

int Heater_read(Heater* this) {
    HeaterState state;
    semTake(this->dataMutex, WAIT_FOREVER);
    state = this->heaterState;
    semGive(this->dataMutex);
    return state;
}

void Heater_write(Heater* this, int newValue) {
    semTake(this->dataMutex, WAIT_FOREVER);
    this->heaterState = newValue;
    semGive(this->dataMutex);
}