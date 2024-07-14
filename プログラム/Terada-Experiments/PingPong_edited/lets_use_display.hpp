lets_use_display.hpp#pragma once

#include <TWELITE>

extern const byte initCommands[25];
extern const int initCommandsLength;

void displayInit(byte);
void displayClear(byte);
void displayLocate(byte, int, int);
void displayDrawChar(byte, int);