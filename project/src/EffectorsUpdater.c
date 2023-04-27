#include "EffectorsUpdater.h"
#include "Effectors.h"
#include "vector.h"

void EffectorsUpdater_init(EffectorsUpdater* this, EffectorsPackage effectorsToUpdate) {
    this->effectors = effectorsToUpdate;

    vector inletValvesVec;
    vector_init(&inletValvesVec, sizeof(ValveState));

    ValveState closed = VALVE_CLOSED;
    vector_push(&inletValvesVec, &closed);
    vector_push(&inletValvesVec, &closed);

    EffectorsStates invalidStates = {
        .valveStates = {
            .outletValveState = VALVE_CLOSED,
            .inletValveStates = vector_to_array(&inletValvesVec),
        },
        .heaterState = HEATER_OFF,
    };

    this->previousStates = invalidStates;
}

void EffectorsUpdater_write(EffectorsUpdater* this, EffectorsStates newStates) {
    // Heater
    if (this->previousStates.heaterState != newStates.heaterState) {
        Heater_write(this->effectors.heater, newStates.heaterState);

        this->previousStates.heaterState = newStates.heaterState;
    }

    // Outlet valve
    if (this->previousStates.valveStates.outletValveState != newStates.valveStates.outletValveState) {
        Valve_write(this->effectors.outletValve, newStates.valveStates.outletValveState);
        
        this->previousStates.valveStates.outletValveState = newStates.valveStates.outletValveState;
    }

    int i;
    const int numInletValves = array_size(&this->effectors.inletValves);
    for (i = 0; i < numInletValves; i++) {
        ValveState* previous = array_get(&this->previousStates.valveStates.inletValveStates, i);
        ValveState* new = array_get(&newStates.valveStates.inletValveStates, i);

        if (*previous != *new) {
            Valve* valve = array_get(&this->effectors.inletValves, i);

            Valve_write(valve, *new);

            *previous = *new;
        }
    }
}
