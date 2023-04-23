#pragma once

#include <time.h>

/**
 * Represents the type of a log message
 */
typedef enum {
    LOG_MESSAGE_WATER_SENSOR = 0,
    LOG_MESSAGE_VALVE = 1,
    LOG_MESSAGE_HEATER = 2
} LogMessageType;

/**************** Water Sensor Log Messages ******************/

/**
 * Represents the ID of the water sensor a message refers to
 */
typedef enum {
    WATER_SENSOR_1_ID = 0,
    WATER_SENSOR_2_ID = 1,
    WATER_SENSOR_3_ID = 2,
    WATER_SENSOR_4_ID = 3
} LogMessageWaterSensorID;

/**
 * Represents an action that can happen with a water sensor
 */
typedef enum {
    ACTION_WATER_SENSOR_ROSE_PAST = 0,
    ACTION_WATER_SENSOR_DROPPED_PAST = 1
} LogMessageWaterSensorAction;

/**
 * Represents the internal data for a water sensor log message
 */
typedef struct {
    LogMessageWaterSensorID sensor;
    LogMessageWaterSensorAction action;
} WaterSensorLogMessage;

/********************* Heater Log Messages *********************/

/**
 * Represents an action that can happen with the heater
 */
typedef enum {
    ACTION_HEATER_OFF = 0,
    ACTION_HEATER_ON = 1
} LogMessageHeaterAction;

/**
 * Represents the internal data for a heater log message
 */
typedef struct {
    LogMessageHeaterAction action;
} HeaterLogMessage;

/********************** Valve Log Messages **********************/

/**
 * Represents the ID of the valve a message refers to
 */
typedef enum {
    INLET_VALVE_1_ID = 0,
    INLET_VALVE_2_ID = 1,
    OUTLET_VALVE_ID = 2,
} LogMessageValveID;

/**
 * Represents an action that can happen with a valve
 */
typedef enum {
    ACTION_VALVE_CLOSE = 0,
    ACTION_VALVE_OPEN = 1
} LogMessageValveAction;

/**
 * Represents the internal data for a valve log message
 */
typedef struct {
    LogMessageValveID valve;
    LogMessageValveAction action;
} ValveLogMessage;

/************************* Log Messages *************************/

/**
 * Represents the time that a log message was sent in ticks and seconds
 */
typedef struct {
    unsigned long ticks;
    struct timespec seconds;
} LogMessageTimestamp;

/**
 * Stores the data contained in a log message based on the type
 */
typedef union {
    WaterSensorLogMessage waterSensorMsg;
    ValveLogMessage valveMsg;
    HeaterLogMessage heaterMsg;
} LogMessageData;

/**
 * Represents the “log” type of visualizer message
 */
typedef struct {
    // The message type
    LogMessageType logMsgType;
    // The data in the message
    LogMessageData logMsgData;
    // When the message was sent
    LogMessageTimestamp msgTimestamp;
} LogMessage;

/**
 * Data: id - The ID of the water sensor this message refers to.
 * action - The action that this message is about.
 * msgTimestamp - The timestamp of when the message was sent.
 * Purpose: This method acts like a constructor for LogMessage of a certain type.
 * Pre-Condition: none.
 * Post-Condition: none.
 * Returns: LogMessage - The new message.
 */
LogMessage LogMessage_WaterSensor(LogMessageWaterSensorID id,
                                  LogMessageWaterSensorAction action,
                                  LogMessageTimestamp timestamp);

/**
 * Data: id - The ID of the valve this message refers to.
 * action - The action that this message is about.
 * msgTimestamp - The timestamp of when the message was sent.
 * Purpose: This method acts like a constructor for LogMessage of a certain type.
 * Pre-Condition: none.
 * Post-Condition: none.
 * Returns: LogMessage - The new message.
 */
LogMessage LogMessage_Valve(LogMessageValveID id,
                            LogMessageValveAction action,
                            LogMessageTimestamp timestamp);

/**
 * Data: action - The action that this message is about.
 * msgTimestamp - The timestamp of when the message was sent.
 * Purpose: This method acts like a constructor for LogMessage of a certain type.
 * Pre-Condition: none.
 * Post-Condition: none.
 * Returns: LogMessage - The new message.
 */
LogMessage LogMessage_Heater(LogMessageHeaterAction action,
                             LogMessageTimestamp timestamp);
