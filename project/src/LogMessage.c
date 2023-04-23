#include "LogMessage.h"

LogMessage LogMessage_WaterSensor(LogMessageWaterSensorID id,
                                  LogMessageWaterSensorAction action,
                                  LogMessageTimestamp timestamp) {
    LogMessage msg = {
        .logMsgType = LOG_MESSAGE_WATER_SENSOR,
        .logMsgData = {
            .waterSensorMsg = {
                .sensor = id,
                .action = action
            }
        },
        .msgTimestamp = timestamp
    };

    return msg;
}

LogMessage LogMessage_Valve(LogMessageValveID id,
                            LogMessageValveAction action,
                            LogMessageTimestamp timestamp) {
    LogMessage msg = {
        .logMsgType = LOG_MESSAGE_WATER_SENSOR,
        .logMsgData = {
            .valveMsg = {
                .valve = id,
                .action = action
            }
        },
        .msgTimestamp = timestamp
    };

    return msg;
}

LogMessage LogMessage_Heater(LogMessageHeaterAction action,
                             LogMessageTimestamp timestamp) {
    LogMessage msg = {
        .logMsgType = LOG_MESSAGE_WATER_SENSOR,
        .logMsgData = {
            .heaterMsg = {
                .action = action
            }
        },
        .msgTimestamp = timestamp
    };

    return msg;
}
