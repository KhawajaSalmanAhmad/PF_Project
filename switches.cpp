#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

int prevTrainX[MAX_TRAINS];
int prevTrainY[MAX_TRAINS];
bool firstTick = true;

void updateSwitchCounters() {
    if (firstTick) {
        for (int i = 0; i < trainCount; i++) {
            prevTrainX[i] = trains[i].x;
            prevTrainY[i] = trains[i].y;
        }
        firstTick = false;
        return;
    }
    
    for (int i = 0; i < trainCount; i++) {
        if (!trains[i].active || trains[i].state != TRAIN_MOVING) {
            continue;
        }
        
        char currentTile = grid[trains[i].y][trains[i].x];
        
        if (isSwitchTile(currentTile)) {
            if (trains[i].x != prevTrainX[i] || trains[i].y != prevTrainY[i]) {
                int switchIdx = getSwitchIndex(currentTile);
                
                if (switches[switchIdx].exists) {
                    switches[switchIdx].counter++;
                }
            }
        }
        
        prevTrainX[i] = trains[i].x;
        prevTrainY[i] = trains[i].y;
    }
}

void queueSwitchFlips() {
    for (int i = 0; i < MAX_SWITCHES; i++) {
        if (switches[i].exists && switches[i].counter >= switches[i].kValue) {
            switches[i].flipQueued = true;
        }
    }
}

void applyDeferredFlips() {
    for (int i = 0; i < MAX_SWITCHES; i++) {
        if (switches[i].flipQueued) {
            switches[i].currentState = 1 - switches[i].currentState;
            switches[i].counter = 0;
            switches[i].flipQueued = false;
            switchFlips++;
        }
    }
}

void updateSignalLights() {
}

void toggleSwitchState(char switchLetter) {
    if (switchLetter >= 'A' && switchLetter <= 'Z') {
        int idx = switchLetter - 'A';
        if (switches[idx].exists) {
            switches[idx].currentState = 1 - switches[idx].currentState;
        }
    }
}

int getSwitchStateForDirection(char switchLetter, int direction) {
    if (switchLetter >= 'A' && switchLetter <= 'Z') {
        int idx = switchLetter - 'A';
        if (switches[idx].exists) {
            return switches[idx].currentState;
        }
    }
    return 0;
}
