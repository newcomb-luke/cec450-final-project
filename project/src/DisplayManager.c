#include "DisplayManager.h"
#include "Effectors.h"
#include "SensorValues.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH  (81)
#define HEIGHT (40)
#define BUFFER_SIZE (WIDTH * HEIGHT)

#define VALVE_WIDTH 4

#define LOG_MESSAGE_WIDTH 38
#define LOG_MESSAGE_LINES 36

#define FLOAT_DISPLAY_WIDTH 7

/****************************** Helper Functions *******************************/

/**
 * Data: dest - The destination pointer to write the float after it is formatted
 *       value - The value to display
 * Purpose: This is a helper function to format a float as 000.00 and write it
 *            to a memory location
 * Pre-Condition: none.
 * Post-Condition: The memory address has had 7 bytes written to it.
 * Returns: none.
 */
void _displayFloat(char* dest, float value);

/****************************** Methods *******************************/

void DisplayManager_init(DisplayManager* this) {
    this->_nextLogMessageLine = 0;
    this->_displayBuffer = malloc(BUFFER_SIZE * sizeof(char));

    const char* template = "                                        |--------------------------------------|\n"
                           "    |----------------------------|      |              Log Messages            |\n"
                           "    |Temperature:          deg C |      |--------------------------------------|\n"
                           "    |----------------------------|      |                                      |\n"
                           "                                        |                                      |\n"
                           "    |----------------------------|      |                                      |\n"
                           "    |Pressure:             kPa   |      |                                      |\n"
                           "    |----------------------------|      |                                      |\n"
                           "                                        |                                      |\n"
                           "    |      |              |      |      |                                      |\n"
                           "    |>====<|              |>====<|      |                                      |\n"
                           "    |      |              |      |      |                                      |\n"
                           "    ==============================      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |- - - - - - - - - - - - - - |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |- - - - - - - - - - - - - - |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |- - - - - - - - - - - - - - |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |- - - - - - - - - - - - - - |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |                            |      |                                      |\n"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |\n"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |\n"
                           "    |============================|      |                                      |\n"
                           "    |         Heater: OFF        |      |                                      |\n"
                           "    ==============================      |                                      |\n"
                           "               |      |                 |                                      |\n"
                           "               |>====<|                 |                                      |\n"
                           "               |      |                 |--------------------------------------|\n";


    memcpy(this->_displayBuffer, template, BUFFER_SIZE);
}

void DisplayManager_setInletValve1(DisplayManager* this, ValveState state) {
    // Start of where the inlet valve 1 is displayed
    const int bufferStart = WIDTH * 10 + 6;

    if (state == VALVE_CLOSED) {
        memset(this->_displayBuffer + bufferStart, '=', VALVE_WIDTH);
    } else {
        memset(this->_displayBuffer + bufferStart, ' ', VALVE_WIDTH);
    }
}

void DisplayManager_setInletValve2(DisplayManager* this, ValveState state) {
    // Start of where the inlet valve 2 is displayed
    const int bufferStart = WIDTH * 10 + 28;

    if (state == VALVE_CLOSED) {
        memset(this->_displayBuffer + bufferStart, '=', VALVE_WIDTH);
    } else {
        memset(this->_displayBuffer + bufferStart, ' ', VALVE_WIDTH);
    }
}

void DisplayManager_setOutletValve(DisplayManager* this, ValveState state) {
    // Start of where the outlet valve is displayed
    const int bufferStart = WIDTH * 38 + 17;

    if (state == VALVE_CLOSED) {
        memset(this->_displayBuffer + bufferStart, '=', VALVE_WIDTH);
    } else {
        memset(this->_displayBuffer + bufferStart, ' ', VALVE_WIDTH);
    }
}

void DisplayManager_setHeater(DisplayManager* this, HeaterState state) {
    // Start of where the heater state is displayed
    const int bufferStart = WIDTH * 35 + 22;
    const char* on = "ON ";
    const char* off = "OFF";
    const int heaterStateStrLen = 3;
    
    if (state == HEATER_ON) {
        memcpy(this->_displayBuffer + bufferStart, on, heaterStateStrLen);
    } else {
        memcpy(this->_displayBuffer + bufferStart, off, heaterStateStrLen);
    }
}

void DisplayManager_setWaterLevel(DisplayManager* this, WaterLevelState state) {
    const int bufferStart = WIDTH * 13 + 5;
    const int totalTankLines = 20;
    const int tankWidth = 28;
    int waterLevel = 0;
    int linesToDraw = 4;

    if (state == WATER_LEVEL_EMPTY) {
        waterLevel = 1;
        linesToDraw = 4;
    } else if (state == WATER_LEVEL_LOW) {
        waterLevel = 7;
        linesToDraw = 3;
    } else if (state == WATER_LEVEL_MID_LOW) {
        waterLevel = 11;
        linesToDraw = 2;
    } else if (state == WATER_LEVEL_MID_HIGH) {
        waterLevel = 15;
        linesToDraw = 1;
    } else if (state == WATER_LEVEL_HIGH) {
        waterLevel = 19;
        linesToDraw = 0;
    }

    // Clears the area above where we need to draw
    int i;
    for (i = 0; i < totalTankLines - waterLevel; i++) {
        memset(this->_displayBuffer + bufferStart + WIDTH * i, ' ', tankWidth);
    }

    // Draws the water level
    for (i = 0; i < waterLevel; i++) {
        memset(this->_displayBuffer + (bufferStart + totalTankLines * WIDTH) - WIDTH * i, '%', tankWidth);
    }

    // Draw the remaining water level sensor lines
    const char* line = "- - - - - - - - - - - - - - ";

    for (i = 0; i < linesToDraw; i++) {
        const int lineBufferStart = WIDTH * 17 + 5;
        const int offset = i * WIDTH * 4;
        memcpy(this->_displayBuffer + lineBufferStart + offset, line, tankWidth);
    }
}

void DisplayManager_setTemperature(DisplayManager* this, float temperature) {
    const int bufferStart = WIDTH * 2 + 19;

    _displayFloat(this->_displayBuffer + bufferStart, temperature);
}

void DisplayManager_setPressure(DisplayManager* this, float pressure) {
    const int bufferStart = WIDTH * 6 + 19;

    _displayFloat(this->_displayBuffer + bufferStart, pressure);
}

void DisplayManager_logMessage(DisplayManager* this, const char* msg) {
    const int bufferStart = WIDTH * 3 + 41;
    
    int currentColumn = 0;
    int i;
    for (i = 0; i < strlen(msg); i++) {
        // Handle newlines by skipping to the next line in the log
        // message area
        if (msg[i] == '\n') {
            currentColumn = 0;
            this->_nextLogMessageLine++;
            continue;
        }

        // Handle scrolling when a message goes past the number of
        // lines available
        if (this->_nextLogMessageLine >= LOG_MESSAGE_LINES) {
            int i;
            // Copy each line to the line before it, discarding the first line
            for (i = 1; i < this->_nextLogMessageLine; i++) {
                int destLineBufferOffset = bufferStart + (i - 1) * WIDTH;
                int srcLineBufferOffset = bufferStart + i * WIDTH;
                char* destPtr = this->_displayBuffer + destLineBufferOffset;
                char* srcPtr = this->_displayBuffer + srcLineBufferOffset;

                memcpy(destPtr, srcPtr, LOG_MESSAGE_WIDTH);
            }

            this->_nextLogMessageLine--;

            char* lastLinePtr = this->_displayBuffer + bufferStart + this->_nextLogMessageLine * WIDTH;

            memset(lastLinePtr, ' ', LOG_MESSAGE_WIDTH);
        }

        // Write the next character
        int bufferOffset = bufferStart + currentColumn + WIDTH * this->_nextLogMessageLine;
        *(this->_displayBuffer + bufferOffset) = msg[i];
        
        currentColumn++;

        // If we are going to go off the end of a line, go to the next one
        if (currentColumn >= LOG_MESSAGE_WIDTH) {
            this->_nextLogMessageLine++;
            currentColumn = 0;
        }
    }

    // If we ended on a newline, or we printed an empty string, then don't 
    // go to the next line, else do so.
    if (currentColumn != 0) {
        this->_nextLogMessageLine++;
    }
}

void DisplayManager_draw(const DisplayManager* this) {
    int i;
    for (i = 0; i < 11; i++) {
        printf("\n");
    }

    printf("%s", this->_displayBuffer);
}

/****************************** Helper Functions *******************************/

void _displayFloat(char* dest, float value) {
    // The design of this supports up to 4 digits before the decimal point
    // and 2 digits after the decimal point.

    char strBuffer[FLOAT_DISPLAY_WIDTH + 1];

    // * means "get the width from the next format specifier", so we use
    // numChars as the total length of the string in the format
    snprintf(strBuffer, FLOAT_DISPLAY_WIDTH + 1, "%*.2f", FLOAT_DISPLAY_WIDTH, value);

    memcpy(dest, strBuffer, FLOAT_DISPLAY_WIDTH);
}
