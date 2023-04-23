#pragma once

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
