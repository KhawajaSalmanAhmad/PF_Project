#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

void trim(char* str) {
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\n' || str[len-1] == '\r')) {
        str[--len] = '\0';
    }
}

bool loadLevelFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "ERROR: Cannot open level file: " << filename << endl;
        return false;
    }
    
    char line[256];
    bool inMap = false;
    int mapRow = 0;
    
    while (file.getline(line, 256)) {
        trim(line);
        
        if (strlen(line) == 0) continue;
        
        if (strncmp(line, "NAME:", 5) == 0) {
            strcpy(levelName, line + 6);
            trim(levelName);
        }
        else if (strncmp(line, "ROWS:", 5) == 0) {
            numRows = atoi(line + 6);
        }
        else if (strncmp(line, "COLS:", 5) == 0) {
            numCols = atoi(line + 6);
        }
        else if (strncmp(line, "SEED:", 5) == 0) {
            seed = atoi(line + 6);
        }
        else if (strncmp(line, "MAP:", 4) == 0) {
            inMap = true;
            mapRow = 0;
        }
        else if (strncmp(line, "SWITCHES:", 9) == 0) {
            inMap = false;
        }
        else if (strncmp(line, "TRAINS:", 7) == 0) {
            inMap = false;
        }
        else if (inMap && mapRow < numRows) {
            int len = strlen(line);
            for (int col = 0; col < numCols && col < len; col++) {
                grid[mapRow][col] = line[col];
                
                if (line[col] == 'S' && spawnCount < MAX_SPAWNS) {
                    spawns[spawnCount].x = col;
                    spawns[spawnCount].y = mapRow;
                    spawns[spawnCount].direction = DIR_RIGHT;
                    spawnCount++;
                }
                
                if (line[col] == 'D' && destCount < MAX_DESTINATIONS) {
                    destinations[destCount].x = col;
                    destinations[destCount].y = mapRow;
                    destCount++;
                }
                
                if (line[col] >= 'A' && line[col] <= 'Z') {
                    int idx = line[col] - 'A';
                    if (!switches[idx].exists) {
                        switches[idx].exists = true;
                        switches[idx].letter = line[col];
                    }
                }
            }
            mapRow++;
        }
        else if (line[0] >= 'A' && line[0] <= 'Z' && strlen(line) > 10) {
            stringstream ss(line);
            char letter;
            string mode;
            int initState, k1, k2, k3, k4;
            
            ss >> letter >> mode >> initState >> k1 >> k2 >> k3 >> k4;
            
            int idx = letter - 'A';
            switches[idx].mode = SWITCH_GLOBAL;
            switches[idx].currentState = initState;
            switches[idx].kValue = k1;
            switches[idx].counter = 0;
            switches[idx].flipQueued = false;
        }
else if (strlen(line) > 5 && line[0] >= '0' && line[0] <= '9') {
    stringstream ss(line);
    int tick, x, y, dir, color;
    
    ss >> tick >> x >> y >> dir >> color;
    
    if (trainCount < MAX_TRAINS) {
        trains[trainCount].id = trainCount;
        trains[trainCount].spawnTick = tick;
        trains[trainCount].x = x;
        trains[trainCount].y = y;
        trains[trainCount].direction = dir;
        trains[trainCount].color = color;
        trains[trainCount].state = TRAIN_WAITING;
        trains[trainCount].active = false;
        trains[trainCount].waitTicks = 0;
        
        // FIX: Find nearest destination, preferring same row
        int bestDest = -1;
        int minDist = 999999;
        
        for (int d = 0; d < destCount; d++)
        {
            int dx = destinations[d].x - x;
            int dy = destinations[d].y - y;
            int dist = abs(dx) + abs(dy);
            
            // Strongly prefer destinations on the same row
            if (dy == 0)
            {
                dist -= 1000;
            }
            
            if (dist < minDist) {
                minDist = dist;
                bestDest = d;
            }
        }
        
        if (bestDest >= 0) {
            trains[trainCount].destX = destinations[bestDest].x;
            trains[trainCount].destY = destinations[bestDest].y;
        } 
        else 
        {
            // Fallback: use first destination
            trains[trainCount].destX = destinations[0].x;
            trains[trainCount].destY = destinations[0].y;
        }
        
        trainCount++;
    }
}
    }
    
    file.close();
    
    cout << "Level loaded: " << levelName << endl;
    cout << "Grid: " << numRows << "x" << numCols << endl;
    cout << "Trains: " << trainCount << endl;
    cout << "Spawns: " << spawnCount << endl;
    cout << "Destinations: " << destCount << endl;
    
    return true;
}

void initializeLogFiles() {
    ofstream trace("out/trace.csv");
    trace << "Tick,TrainID,X,Y,Direction,State\n";
    trace.close();
    
    ofstream sw("out/switches.csv");
    sw << "Tick,Switch,Mode,State\n";
    sw.close();
    
    ofstream sig("out/signals.csv");
    sig << "Tick,Switch,Signal\n";
    sig.close();
}

void logTrainTrace() {
    ofstream trace("out/trace.csv", ios::app);
    
    for (int i = 0; i < trainCount; i++) {
        if (trains[i].active) {
            trace << currentTick << ","
                  << trains[i].id << ","
                  << trains[i].x << ","
                  << trains[i].y << ","
                  << trains[i].direction << ","
                  << trains[i].state << "\n";
        }
    }
    
    trace.close();
}

void logSwitchState() {
    ofstream sw("out/switches.csv", ios::app);
    
    for (int i = 0; i < MAX_SWITCHES; i++) {
        if (switches[i].exists) {
            sw << currentTick << ","
               << switches[i].letter << ","
               << "GLOBAL" << ","
               << switches[i].currentState << "\n";
        }
    }
    
    sw.close();
}

void logSignalState() {
    ofstream sig("out/signals.csv", ios::app);
    
    for (int i = 0; i < MAX_SWITCHES; i++) {
        if (switches[i].exists) {
            sig << currentTick << ","
                << switches[i].letter << ","
                << "GREEN\n";
        }
    }
    
    sig.close();
}

void writeMetrics() {
    ofstream metrics("out/metrics.txt");
    
    metrics << "=== SWITCHBACK RAILS METRICS ===\n\n";
    metrics << "Level: " << levelName << "\n";
    metrics << "Total Ticks: " << currentTick << "\n\n";
    
    metrics << "Trains Delivered: " << trainsDelivered << "\n";
    metrics << "Trains Crashed: " << trainsCrashed << "\n";
    metrics << "Total Trains: " << trainCount << "\n\n";
    
    metrics << "Switch Flips: " << switchFlips << "\n";
    
    if (trainsDelivered > 0) {
        metrics << "Average Wait Time: " << (totalWaitTicks / trainsDelivered) << " ticks\n";
    }
    
    if (currentTick > 0) {
        metrics << "Throughput: " << (trainsDelivered * 100 / currentTick) << " trains per 100 ticks\n";
    }
    
    metrics.close();
    
    cout << "\n=== SIMULATION COMPLETE ===\n";
    cout << "Delivered: " << trainsDelivered << "/" << trainCount << endl;
    cout << "Crashed: " << trainsCrashed << endl;
    cout << "Metrics saved to out/metrics.txt\n";
}

