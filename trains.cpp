#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>
#include <cmath>

struct PlannedMove {
    int trainIdx;
    int nextX;
    int nextY;
    int nextDir;
    bool valid;
};

PlannedMove plannedMoves[MAX_TRAINS];

void spawnTrainsForTick() {
    for (int i = 0; i < trainCount; i++) {
        if (!trains[i].active && trains[i].spawnTick == currentTick) {
            bool occupied = false;
            for (int j = 0; j < trainCount; j++) {
                if (trains[j].active && 
                    trains[j].x == trains[i].x && 
                    trains[j].y == trains[i].y) {
                    occupied = true;
                    break;
                }
            }
            
            if (!occupied) {
                trains[i].active = true;
                trains[i].state = TRAIN_MOVING;
                trains[i].waitTicks = 0;
            }
        }
    }
}

int calculateDistance(int trainIdx, int x, int y) {
    int destX = trains[trainIdx].destX;
    int destY = trains[trainIdx].destY;
    return abs(x - destX) + abs(y - destY);
}

int getNextDirection(int x, int y, char tile, int currentDir) {
    if (tile == '/') {
        if (currentDir == DIR_RIGHT) return DIR_UP;
        if (currentDir == DIR_LEFT) return DIR_DOWN;
        if (currentDir == DIR_UP) return DIR_RIGHT;
        if (currentDir == DIR_DOWN) return DIR_LEFT;
    }
    else if (tile == '\\') {
        if (currentDir == DIR_RIGHT) return DIR_DOWN;
        if (currentDir == DIR_LEFT) return DIR_UP;
        if (currentDir == DIR_UP) return DIR_LEFT;
        if (currentDir == DIR_DOWN) return DIR_RIGHT;
    }
    else if (isSwitchTile(tile)) {
        int idx = getSwitchIndex(tile);
        int state = switches[idx].currentState;
        
        if (state == 0) {
            return currentDir;
        } else {
            return (currentDir + 1) % 4;
        }
    }
    
    return currentDir;
}

int getSmartDirectionAtCrossing(int trainIdx, int x, int y, int currentDir) {
    int destX = trains[trainIdx].destX;
    int destY = trains[trainIdx].destY;
    
    int dx = destX - x;
    int dy = destY - y;
    
    if (abs(dx) > abs(dy)) {
        if (dx > 0) return DIR_RIGHT;
        if (dx < 0) return DIR_LEFT;
    }
    
    if (dy > 0) return DIR_DOWN;
    if (dy < 0) return DIR_UP;
    
    return currentDir;
}

bool determineNextPosition(int trainIdx, int& nextX, int& nextY, int& nextDir) {
    Train& t = trains[trainIdx];
    
    if (t.waitTicks > 0) {
        nextX = t.x;
        nextY = t.y;
        nextDir = t.direction;
        return false;
    }
    
    nextX = t.x;
    nextY = t.y;
    nextDir = t.direction;
    
    if (t.direction == DIR_UP) nextY--;
    else if (t.direction == DIR_DOWN) nextY++;
    else if (t.direction == DIR_LEFT) nextX--;
    else if (t.direction == DIR_RIGHT) nextX++;
    
    if (!isInBounds(nextX, nextY)) {
        return false;
    }
    
    char nextTile = grid[nextY][nextX];
    
    if (!isTrackTile(nextTile)) {
        return false;
    }
    
    nextDir = getNextDirection(nextX, nextY, nextTile, t.direction);
    
    if (nextTile == '+') {
        nextDir = getSmartDirectionAtCrossing(trainIdx, nextX, nextY, t.direction);
    }
    
    return true;
}

void determineAllRoutes() {
    for (int i = 0; i < trainCount; i++) {
        plannedMoves[i].trainIdx = i;
        plannedMoves[i].valid = false;
        
        if (trains[i].active && trains[i].state == TRAIN_MOVING) {
            plannedMoves[i].valid = determineNextPosition(
                i,
                plannedMoves[i].nextX,
                plannedMoves[i].nextY,
                plannedMoves[i].nextDir
            );
        }
    }
}

void detectCollisions() {
    for (int i = 0; i < trainCount; i++) {
        if (!plannedMoves[i].valid) continue;
        
        for (int j = i + 1; j < trainCount; j++) {
            if (!plannedMoves[j].valid) continue;
            
            if (plannedMoves[i].nextX == plannedMoves[j].nextX &&
                plannedMoves[i].nextY == plannedMoves[j].nextY) {
                
                trains[i].state = TRAIN_CRASHED;
                trains[j].state = TRAIN_CRASHED;
                plannedMoves[i].valid = false;
                plannedMoves[j].valid = false;
                trainsCrashed += 2;
            }
            
            if (trains[i].x == plannedMoves[j].nextX &&
                trains[i].y == plannedMoves[j].nextY &&
                trains[j].x == plannedMoves[i].nextX &&
                trains[j].y == plannedMoves[i].nextY) {
                
                trains[i].state = TRAIN_CRASHED;
                trains[j].state = TRAIN_CRASHED;
                plannedMoves[i].valid = false;
                plannedMoves[j].valid = false;
                trainsCrashed += 2;
            }
        }
    }
}

void moveAllTrains() {
    for (int i = 0; i < trainCount; i++) {
        if (plannedMoves[i].valid && trains[i].state == TRAIN_MOVING) {
            trains[i].x = plannedMoves[i].nextX;
            trains[i].y = plannedMoves[i].nextY;
            trains[i].direction = plannedMoves[i].nextDir;
            
            char tile = grid[trains[i].y][trains[i].x];
            if (tile == '=') {
                trains[i].waitTicks = 1;
            }
        }
        
        if (trains[i].waitTicks > 0) {
            trains[i].waitTicks--;
            totalWaitTicks++;
        }
    }
}

void checkArrivals() {
    for (int i = 0; i < trainCount; i++) {
        if (trains[i].active && trains[i].state == TRAIN_MOVING) {
            if (trains[i].x == trains[i].destX && 
                trains[i].y == trains[i].destY) {
                trains[i].state = TRAIN_ARRIVED;
                trains[i].active = false;
                trainsDelivered++;
            }
        }
    }
}

void applyEmergencyHalt() {
}

void updateEmergencyHalt() {
}
