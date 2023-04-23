#include "DisplayManager.h"
#include "Effectors.h"
#include "SensorValues.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH  (80)
#define HEIGHT (40)
#define BUFFER_SIZE (WIDTH * HEIGHT)

#define VALVE_WIDTH 4

void DisplayManager_init(DisplayManager* this) {
    this->_displayBuffer = malloc(BUFFER_SIZE * sizeof(char));

    const char* template = "                                        |--------------------------------------|"
                           "    |----------------------------|      |              Log Messages            |"
                           "    |Temperature:    23.00 deg C |      |--------------------------------------|"
                           "    |----------------------------|      |                                      |"
                           "                                        |                                      |"
                           "    |----------------------------|      |                                      |"
                           "    |Pressure:      101.32 kPa   |      |                                      |"
                           "    |----------------------------|      |                                      |"
                           "                                        |                                      |"
                           "    |      |              |      |      |                                      |"
                           "    |>====<|              |>====<|      |                                      |"
                           "    |      |              |      |      |                                      |"
                           "    ==============================      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |- - - - - - - - - - - - - - |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |- - - - - - - - - - - - - - |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |- - - - - - - - - - - - - - |      |                                      |"
                           "    |                            |      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |%%%%%%%%%%%%%%%%%%%%%%%%%%%%|      |                                      |"
                           "    |============================|      |                                      |"
                           "    |         Heater: OFF        |      |                                      |"
                           "    ==============================      |                                      |"
                           "               |      |                 |                                      |"
                           "               |>====<|                 |                                      |"
                           "               |      |                 |--------------------------------------|";


    memcpy(this->_displayBuffer, template, BUFFER_SIZE);
}

void DisplayManager_draw(const DisplayManager* this) {
    char lineBuffer[WIDTH + 1];

    int i = 0;
    for (i = 0; i < 14; i++) {
        printf("\n");
    }

    i = 0;
    for (i = 0; i < HEIGHT; i++) {
        memcpy(lineBuffer, this->_displayBuffer + WIDTH * i, WIDTH * sizeof(char));
        lineBuffer[WIDTH] = '\0';

        printf("%s\n", lineBuffer);
    }
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

    int i;
    for (i = 0; i < totalTankLines - waterLevel; i++) {
        memset(this->_displayBuffer + bufferStart + WIDTH * i, ' ', tankWidth);
    }
    for (i = 0; i < waterLevel; i++) {
        memset(this->_displayBuffer + (bufferStart + totalTankLines * WIDTH) - WIDTH * i, '%', tankWidth);
    }

    const char* line = "- - - - - - - - - - - - - - ";


    for (i = 0; i < linesToDraw; i++) {
        const int lineBufferStart = WIDTH * 17 + 5;
        const int offset = i * WIDTH * 4;
        memcpy(this->_displayBuffer + lineBufferStart + offset, line, tankWidth);
    }
}

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

void DisplayManager_setTemperature(DisplayManager* this, float temperature) {
    const int bufferStart = WIDTH * 2 + 19;

    _displayFloat(this->_displayBuffer + bufferStart, temperature);
}

void DisplayManager_setPressure(DisplayManager* this, float pressure) {
    const int bufferStart = WIDTH * 6 + 19;

    _displayFloat(this->_displayBuffer + bufferStart, pressure);
}

void _displayFloat(char* dest, float value) {
    // The design of this supports up to 4 digits before the decimal point
    // and 2 digits after the decimal point.

    const int numChars = 7;
    char strBuffer[numChars + 1];

    // * means "get the width from the next format specifier", so we use
    // numChars as the total length of the string in the format
    snprintf(strBuffer, numChars + 1, "%*.2f", numChars, value);

    memcpy(dest, strBuffer, numChars);
}
