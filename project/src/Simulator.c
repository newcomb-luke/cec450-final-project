#include <stdio.h>
#include <math.h>
#include "Effectors.h"
#include "EffectorsMonitor.h"
#include "Simulator.h"
#include "PressureSim.h"
#include "vector.h"
#include <taskLib.h>
#include <vxworks.h>
#include <time.h>

#define TANK_RADIUS                (17.84124)
#define TANK_HEIGHT                (50.0) 
#define TANK_VOLUME                (((M_PI * TANK_RADIUS * TANK_RADIUS) * TANK_HEIGHT) / 1000.0)

#define BASE_TANK_PRESSURE         (140.0)
#define EXPLOSION_TANK_PRESSURE    (360.0)

#define INLET_VALVE_RATE           (0.5)
#define OUTLET_VALVE_RATE          (0.5)

#define INLET_WATER_TEMP           (20.0)
#define LITERS_PER_CUBIC_METER     (1000)
#define DENSITY_OF_WATER           (1000)
#define WATER_BOILING_POINT        (100.0)
#define HEATER_WATTAGE             (4000.0)
#define WATER_SPECIFIC_HEAT        (100.0)

#define SIMULATOR_LOOP_DELAY       (5)
#define SIMULATOR_TASK_PRIORITY    (100)

float calculateMassOfWater(float waterVolume);

void _Simulator_loop(Simulator* this);
float _Simulator_calculateInletRate(Simulator* this, ValveStates states);
float _Simulator_calculateMixedWaterTemp(Simulator* this, float tankTemperature, float inletMass);
float _Simulator_calculateWaterHeating(Simulator* this, HeaterState state, float deltaTime);


void Simulator_init(Simulator* this, EffectorsPackage effectors, SensorsPackage sensors){
    EffectorsMonitor_init(&this->_effectorsMonitor, effectors);

    SensorsUpdater_init(&this->_sensorsUpdater,sensors);

    PressureSim_init(&this->_pressureSim);

    this->_waterLevel = 0.0;

    this->_taskID = NULL;
}

void Simulator_start(Simulator* this){
    this->_taskID = taskSpawn("simulator", 
                              SIMULATOR_TASK_PRIORITY, 
                              0, 0x2000,
                              (FUNCPTR) _Simulator_loop, (size_t) this,
                              0, 0, 0, 0, 0, 0, 0, 0, 0);
}

/**
 * Purpose: This method is the Simulator’s main loop that is the entry point of a task.
 * Pre-Condition: Simulator is initialized. Simulator is started.
 * Post-Condition: none.
 * Returns: never.
 */
void _Simulator_loop(Simulator* this) {
    struct timespec lastTimestamp;
    clock_gettime(CLOCK_REALTIME, &lastTimestamp);

    // Initialize our EffectorsStates variable
    vector inletValveStatesVec;
    vector_init(&inletValveStatesVec, sizeof(ValveState));
    ValveState closed = VALVE_CLOSED;
    vector_push(&inletValveStatesVec, &closed);
    vector_push(&inletValveStatesVec, &closed);
    EffectorsStates effectorStates;
    effectorStates.valveStates.inletValveStates = vector_to_array(&inletValveStatesVec);
    effectorStates.heaterState = HEATER_OFF;
    effectorStates.valveStates.outletValveState = VALVE_CLOSED;

    float temperature = INLET_WATER_TEMP;

    // Infinite loop
    for (;;) {
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        struct timespec deltaTimeTimespec = subtractTimespecs(lastTimestamp, now);
        float deltaTime = timespecToSeconds(deltaTimeTimespec);
        lastTimestamp = now;

        EffectorsMonitor_read(&(this->_effectorsMonitor), &effectorStates);

        float inletRate = _Simulator_calculateInletRate(this, effectorStates.valveStates);
        float inletWater = inletRate * deltaTime;

        float inletMass = calculateMassOfWater(inletWater);
        temperature = _Simulator_calculateMixedWaterTemp(this, temperature, inletMass);

        this->_waterLevel += inletWater;
        this->_waterLevel = float_max(float_min(this->_waterLevel, TANK_VOLUME), 0.0);

        float tempDelta = _Simulator_calculateWaterHeating(this, effectorStates.heaterState, deltaTime);
        temperature = float_min(temperature + tempDelta, WATER_BOILING_POINT);
        
        float pressure = PressureSim_update(&this->_pressureSim, 
                                            effectorStates.valveStates.outletValveState,
                                            temperature,
                                            this->_waterLevel,
                                            deltaTime) + BASE_TANK_PRESSURE;

        SensorUpdates sensorUpdates;
        sensorUpdates.waterLevel = this->_waterLevel;
        sensorUpdates.temperature = temperature;
        sensorUpdates.pressure = pressure;

        SensorsUpdater_write(&this->_sensorsUpdater, sensorUpdates);

        if (pressure > EXPLOSION_TANK_PRESSURE){
            printf("TANK EXPLODED\n");
            break;
        }

        taskDelay(SIMULATOR_LOOP_DELAY);  
    }

    for (;;) {}
}

/**
 * Purpose: This method calculates the inlet rate of water into the tank using valve states.
 * Pre-Condition: Simulator is initialized. Simulator is started.
 * Post-Condition: none.
 * Returns: float - the inlet rate in Liters per second.
 */
float _Simulator_calculateInletRate(Simulator* this, ValveStates states) {
    float inletRate = 0.0;

    const int numInletValves = array_size(&states.inletValveStates);
    int i;
    for (i = 0; i < numInletValves; i++){ 
        ValveState* state = array_get(&states.inletValveStates, i);

        if (*state == VALVE_OPEN) {
            inletRate += INLET_VALVE_RATE;
        }
    }

    if (states.outletValveState == VALVE_OPEN) {
        inletRate -= OUTLET_VALVE_RATE;
    }


    return inletRate;

}

/**
 * Data: tankTemperature - The temperature of the water in the tank in degrees Celsius
 *       inletMass - The mass of the current amount of water that is entering the tank.
 * Purpose: This method calculates the final temperature of mixing inlet water into the tank.
 * Pre-Condition: Simulator is initialized. Simulator is started.
 * Post-Condition: none.
 * Returns: float - the final temperature after mixing.
 */
float _Simulator_calculateMixedWaterTemp(Simulator* this, float tankTemperature, float inletMass) {
    float newTankTemp = tankTemperature;
    float tankMass = calculateMassOfWater(this->_waterLevel);

    if (inletMass > 0.0 && tankMass > 0.0) {
        float kgDegreesK = tankMass * tankTemperature + inletMass * INLET_WATER_TEMP;
        newTankTemp = kgDegreesK / (tankMass + inletMass);
    }

    return newTankTemp;
}

/**
 * Data: state - The state of the heater
 *       deltaTime - The time in seconds since the last update
 * Purpose: This method calculates the change in water temperature for this next update.
 * Pre-Condition: Simulator is initialized. Simulator is started.
 * Post-Condition: none.
 * Returns: float - the change in water temperature due to the heater.
 */
float _Simulator_calculateWaterHeating(Simulator* this, HeaterState state, float deltaTime) {
    float deltaTemp = 0.0;

    if (state == HEATER_ON) {
        deltaTemp = (deltaTime * HEATER_WATTAGE) / (this->_waterLevel * WATER_SPECIFIC_HEAT);
    }

    return deltaTemp;
}

float calculateMassOfWater(float waterVolume){
    float volumeCubicMeters = waterVolume / LITERS_PER_CUBIC_METER;
    float waterMass = volumeCubicMeters * DENSITY_OF_WATER;

    return waterMass;
}

struct timespec subtractTimespecs(struct timespec before, struct timespec after) {
    struct timespec result;

    result.tv_sec = after.tv_sec - before.tv_sec;
    result.tv_nsec = after.tv_nsec - before.tv_nsec;

    if (result.tv_nsec < 0) {
        result.tv_sec--;

        result.tv_nsec = 1000000000 - result.tv_nsec;
    }

    return result;
}

float timespecToSeconds(struct timespec spec) {
    return (float)(spec.tv_sec) + (float)(spec.tv_nsec / (float)(1000000000.0));
}

float float_min(float a, float b) {
    if (a <= b) {
        return a;
    }

    return b;
}

float float_max(float a, float b) {
    if (a >= b) {
        return a;
    }

    return b;
}
