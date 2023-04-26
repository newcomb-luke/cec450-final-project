#pragma once

typedef struct {
    float _pressureFromTemperature;
} PressureSim;


void PressureSim_init(PressureSim* this);
float PressureSim_update(PressureSim* this, ValveState outletValveState, float temperature, float waterLevel, float deltaTime);
float calculateOutletValvePressureRelief(double outletValve, double pressureFromVolume, double pressureFromTemperature);
