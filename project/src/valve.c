#include <semlib.h>

enum ValveState {
    Closed = 0,
    Open = 1
};

void Valve_init(Valve* this) {
    this->valveState = Closed;
    this->dataMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}

int Valve_read(Valve* this) {
    ValveState state;
    semTake(this->dataMutex, WAIT_FOREVER);
    state = this->valveState;
    semGive(this->dataMutex);
    return state;
}

void Valve_write(Valve* this, int newValue) {
    semTake(this->dataMutex, WAIT_FOREVER);
    this->valveState = newValue;
    semGive(this->dataMutex);
}