#include "EffectorsMonitor.h"
#include "Effectors.h"

void EffectorsMonitor_init(EffectorsMonitor* this, EffectorsPackage effectorsToMonitor) {
    this->effectors = effectorsToMonitor;
}

void EffectorsMonitor_read(EffectorsMonitor* this, EffectorsStates* states) {
    // Read inlet valves
    int i;
    const int numInletValves = array_size(&this->effectors.inletValves);
    for (i = 0; i < numInletValves; i++) {
        ValveState* valveState = array_get(&states->valveStates.inletValveStates, i);
        Valve* valve = array_get(&this->effectors.inletValves, i);

        *valveState = Valve_read(valve);
    }

    // Read outlet valve
    states->valveStates.outletValveState = Valve_read(this->effectors.outletValve);

    // Read heater
    states->heaterState = Heater_read(this->effectors.heater);
}
