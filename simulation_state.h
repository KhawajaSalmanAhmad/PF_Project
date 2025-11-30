#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H
#include "simulation_state.h"
#include <cstring>

const int MAX_ROWS = 50;
const int MAX_COLS = 50;
const int MAX_TRAINS = 100;
const int MAX_SWITCHES = 26;
const int MAX_SPAWNS = 20;
const int MAX_DESTINATIONS = 20;
const int MAX_NAME_LEN = 100;

const int DIR_UP = 0;
const int DIR_RIGHT = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;

const int TRAIN_WAITING = 0;
const int TRAIN_MOVING = 1;
const int TRAIN_ARRIVED = 2;
const int TRAIN_CRASHED = 3;

const int SWITCH_GLOBAL = 0;
const int SWITCH_PER_DIR = 1;

struct Train {
    int id;
    int x, y;
    int direction;
    int color;
    int state;
    int spawnTick;
    int destX, destY;
    int waitTicks;
    bool active;
};

struct Switch {
    char letter;
    int mode;
    int currentState;
    int kValue;
    int counter;
    bool flipQueued;
    bool exists;
};

struct SpawnPoint {
    int x, y;
    int direction;
};

struct DestPoint {
    int x, y;
};

extern char grid[MAX_ROWS][MAX_COLS];
extern int numRows;
extern int numCols;

extern Train trains[MAX_TRAINS];
extern int trainCount;

extern Switch switches[MAX_SWITCHES];

extern SpawnPoint spawns[MAX_SPAWNS];
extern int spawnCount;
extern DestPoint destinations[MAX_DESTINATIONS];
extern int destCount;

extern char levelName[MAX_NAME_LEN];
extern int seed;
extern int currentTick;
extern bool simulationRunning;

extern int trainsDelivered;
extern int trainsCrashed;
extern int totalWaitTicks;
extern int switchFlips;

void initializeSimulationState();
#endif
