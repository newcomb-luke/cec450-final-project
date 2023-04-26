#pragma once

#include "array.h"
#include <semLib.h>

/**
 * Represents the possible states of the heater
 */
typedef enum {
    HEATER_OFF = 0,
    HEATER_ON = 1
} HeaterState;

/**
 * Represents the possible states of a valve
 */
typedef enum {
    VALVE_CLOSED = 0,
    VALVE_OPEN = 1
} ValveState;

/**
 * Stores a collection of valve states, inlet and outlet
 */
typedef struct {
    array inletValveStates;
    ValveState outletValveState;
} ValveStates;

/**
 * Stores a collection of effector states
 */
typedef struct {
    HeaterState heaterState;
    ValveStates valveStates;
} EffectorsStates;





/**
 * Allows exclusive access and modification of a valve state
 */
typedef struct {
    // Mutex used to protect the data in this structure
    SEM_ID _dataMutex;
    // The current state of the valve
    ValveState _state;
} Valve;

/**
 * Purpose: This method initializes a Valve class and acts like a constructor.
 * Pre-Condition: Valve is uninitialized.
 * Post-Condition: dataMutex is initialized and initially unlocked. state is set to VALVE_CLOSED. Valve is initialized.
 * Returns: none.
 */
void Valve_init(Valve* this);

/**
 * Purpose: This method reads the current state of the valve.
 * Pre-Condition: Valve is initialized.
 * Post-Condition: none.
 * Returns: ValveState - the state of the valve
 */
ValveState Valve_read(Valve* this);

/**
 * Data: newState - The new state for the valve
 * Purpose: This method sets a new state for the valve.
 * Pre-Condition: Valve is initialized.
 * Post-Condition: none.
 * Returns: none.
 */
void Valve_write(Valve* this, ValveState newState);




/**
 * Allows exclusive access and modification of a heater state
 */
typedef struct {
    // Mutex used to protect the data in this structure
    SEM_ID _dataMutex;
    // The current state of the heater
    HeaterState _state;
} Heater;

/**
 * Purpose: This method initializes a Heater class and acts like a constructor.
 * Pre-Condition: Heater is uninitialized.
 * Post-Condition: dataMutex is initialized and initially unlocked. state is set to HEATER_OFF. Heater is initialized.
 * Returns: none.
 */
void Heater_init(Heater* this);

/**
 * Purpose: This method reads the current state of the heater.
 * Pre-Condition: Heater is initialized.
 * Post-Condition: none.
 * Returns: HeaterState - the state of the heater
 */
HeaterState Heater_read(Heater* this);

/**
 * Data: newState - The new state for the heater
 * Purpose: This method sets a new state for the heater.
 * Pre-Condition: Heater is initialized.
 * Post-Condition: none.
 * Returns: none.
 */
void Heater_write(Heater* this, HeaterState newState);




/**
 * Stores a collection of effectors to be either monitored or updated
 */
typedef struct {
    Heater* heater;
    // Array of Valve*
    array inletValves;
    Valve* outletValve;
} EffectorsPackage;
