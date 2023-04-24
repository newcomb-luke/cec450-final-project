#include "VisualizerMessaage.h"

VisualizerMessage VisualizerMessage_Log(LogMessage msg) {
    VisualizerMessage visMsg = {
        .msgType = VISUALIZER_LOG_MESSAGE,
        .msgData = {
            .logMsg = msg,
        }
    };
    
    return visMsg;
}

VisualizerMessage VisualizerMessage_Update(UpdateMessage msg) {
    VisualizerMessage visMsg = {
        .msgType = VISUALIZER_UPDATE_MESSAGE,
        .msgData = {
            .updateMsg = msg,
        }
    };
    
    return visMsg;
}
