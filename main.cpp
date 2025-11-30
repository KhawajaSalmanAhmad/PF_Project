#include <iostream>
#include <cstdlib>
#include "core/simulation_state.h"
#include "core/io.h"
#include "core/grid.h"
#include "core/trains.h"
#include "core/switches.h"

using namespace std;

void runSimulation(int maxTicks = 1000) {
    cout << "=== Starting Switchback Rails Simulation ===" << endl;
    cout << "Level: " << levelName << endl;
    cout << "Grid Size: " << numRows << " x " << numCols << endl;
    cout << "Seed: " << seed << endl << endl;

    initializeLogFiles();
    
    simulationRunning = true;
    
    while (simulationRunning && currentTick < maxTicks) {
        cout << "\n========== TICK " << currentTick << " ==========" << endl;
        
        spawnTrainsForTick();
        determineAllRoutes();
        updateSwitchCounters();
        queueSwitchFlips();
        moveAllTrains();
        checkArrivals();
        applyDeferredFlips();
        updateSignalLights();
        printGrid();
        
        logTrainTrace();
        logSwitchState();
        logSignalState();
        
        bool allDone = true;
        for (int i = 0; i < trainCount; i++) {
            if (trains[i].active && 
                trains[i].state != TRAIN_ARRIVED && 
                trains[i].state != TRAIN_CRASHED) {
                allDone = false;
                break;
            }
        }
        
        if (allDone && currentTick > 0) {
            cout << "\n=== All trains completed ===" << endl;
            simulationRunning = false;
        }
        
        currentTick++;
    }
    
    writeMetrics();
    
    cout << "\n=== Simulation Complete ===" << endl;
    cout << "Trains Delivered: " << trainsDelivered << endl;
    cout << "Trains Crashed: " << trainsCrashed << endl;
    cout << "Total Ticks: " << currentTick << endl;
    cout << "Switch Flips: " << switchFlips << endl;
    
    if (trainCount > 0) {
        float avgWait = (float)totalWaitTicks / trainCount;
        cout << "Average Wait Time: " << avgWait << " ticks" << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <level_file.lvl>" << endl;
        cout << "Example: " << argv[0] << " data/levels/easy_level.lvl" << endl;
        return 1;
    }
    
    const char* levelFile = argv[1];
    
    cout << "Loading level file: " << levelFile << endl;
    
    initializeSimulationState();
    
    if (!loadLevelFile(levelFile)) {
        cerr << "Error: Failed to load level file!" << endl;
        return 1;
    }
    
    cout << "Level loaded successfully!" << endl;
    
    runSimulation();
    
    return 0;
}

