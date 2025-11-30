#include "simulation_state.h"
#include <cstring>

char grid[MAX_ROWS][MAX_COLS];
int numRows = 0;
int numCols = 0;

Train trains[MAX_TRAINS];
int trainCount = 0;

Switch switches[MAX_SWITCHES];

SpawnPoint spawns[MAX_SPAWNS];
int spawnCount = 0;

DestPoint destinations[MAX_DESTINATIONS];
int destCount = 0;

char levelName[MAX_NAME_LEN];
int seed = 0;
int currentTick = 0;
bool simulationRunning = false;

int trainsDelivered = 0;
int trainsCrashed = 0;
int totalWaitTicks = 0;
int switchFlips = 0;

void initializeSimulationState() {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            grid[i][j] = '.';
        }
    }
    numRows = 0;
    numCols = 0;
    
    for (int i = 0; i < MAX_TRAINS; i++) {
        trains[i].active = false;
        trains[i].state = TRAIN_WAITING;
        trains[i].waitTicks = 0;
    }
    trainCount = 0;
    
    for (int i = 0; i < MAX_SWITCHES; i++) {
        switches[i].exists = false;
        switches[i].counter = 0;
        switches[i].flipQueued = false;
        switches[i].currentState = 0;
    }
    
    spawnCount = 0;
    destCount = 0;
    
    memset(levelName, 0, MAX_NAME_LEN);
    seed = 0;
    currentTick = 0;
    simulationRunning = false;
    
    trainsDelivered = 0;
    trainsCrashed = 0;
    totalWaitTicks = 0;
    switchFlips = 0;
}
