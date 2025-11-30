#ifndef SWITCHES_H
#define SWITCHES_H

void updateSwitchCounters();

void queueSwitchFlips();

void applyDeferredFlips();

void updateSignalLights();

void toggleSwitchState(char switchLetter);

int getSwitchStateForDirection(char switchLetter, int direction);

#endif
