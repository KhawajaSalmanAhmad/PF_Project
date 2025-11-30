
#include "grid.h"
#include "simulation_state.h"
#include <iostream>

using namespace std;

bool isInBounds(int x, int y) {
    return (x >= 0 && x < numCols && y >= 0 && y < numRows);
}

bool isTrackTile(char tile) {
    return (tile == '-' || tile == '|' || 
            tile == '/' || tile == '\\' || 
            tile == '+' || tile == 'S' || 
            tile == 'D' || tile == '=' ||
            (tile >= 'A' && tile <= 'Z'));
}

bool isSwitchTile(char tile) {
    return (tile >= 'A' && tile <= 'Z');
}

int getSwitchIndex(char tile) {
    if (tile >= 'A' && tile <= 'Z') {
        return tile - 'A';
    }
    return -1;
}

bool isSpawnPoint(int x, int y) {
    for (int i = 0; i < spawnCount; i++) {
        if (spawns[i].x == x && spawns[i].y == y) {
            return true;
        }
    }
    return false;
}

bool isDestinationPoint(int x, int y) {
    for (int i = 0; i < destCount; i++) {
        if (destinations[i].x == x && destinations[i].y == y) {
            return true;
        }
    }
    return false;
}

bool toggleSafetyTile(int x, int y) {
    if (!isInBounds(x, y)) {
        return false;
    }
    
    if (grid[y][x] == '=') {
        grid[y][x] = '-';
        return true;
    } else if (grid[y][x] == '-' || grid[y][x] == '|') {
        grid[y][x] = '=';
        return true;
    }
    
    return false;
}

void printGrid() {
    cout << "\n=== TICK " << currentTick << " ===" << endl;
    
    for (int y = 0; y < numRows; y++) {
        for (int x = 0; x < numCols; x++) {
            bool trainHere = false;
            
            for (int t = 0; t < trainCount; t++) {
                if (trains[t].active && 
                    trains[t].x == x && 
                    trains[t].y == y &&
                    trains[t].state != TRAIN_ARRIVED) {
                    
                    cout << trains[t].id % 10;
                    trainHere = true;
                    break;
                }
            }
            
            if (!trainHere) {
                cout << grid[y][x];
            }
        }
        cout << endl;
    }
    
    cout << "\nActive Trains:" << endl;
    for (int t = 0; t < trainCount; t++) {
        if (trains[t].active && trains[t].state == TRAIN_MOVING) {
            const char* dirName[] = {"UP", "RIGHT", "DOWN", "LEFT"};
            cout << "  Train " << trains[t].id << " at (" << trains[t].x 
                 << "," << trains[t].y << ") moving " << dirName[trains[t].direction] << endl;
        }
    }
    
    cout << endl;
}
