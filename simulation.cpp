#include <unistd.h>
#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include "grid.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void initializeSimulation() {
    initializeSimulationState();
    srand(seed);
    initializeLogFiles();
    simulationRunning = true;
    currentTick = 0;
    
    cout << endl;
    cout << "=========================================" << endl;
    cout << "  SWITCHBACK RAILS SIMULATION START" << endl;
    cout << "=========================================" << endl;
    cout << "Level: " << levelName << endl;
    cout << "Grid Size: " << numRows << " x " << numCols << endl;
    cout << "Total Trains: " << trainCount << endl;
    cout << "Seed: " << seed << endl;
    cout << "=========================================" << endl;
}

void simulateOneTick() {
    spawnTrainsForTick();
    determineAllRoutes();
    updateSwitchCounters();
    queueSwitchFlips();
    detectCollisions();
    moveAllTrains();
    checkArrivals();
    applyDeferredFlips();
    printGrid();
    logTrainTrace();
    logSwitchState();
    logSignalState();
    currentTick++;
     usleep(500000);  
}

bool isSimulationComplete() {
    int activeTrainCount = 0;
    int deliveredCount = 0;
    int crashedCount = 0;
    
    for (int trainIndex = 0; trainIndex < trainCount; trainIndex++) {
        if (trains[trainIndex].state == TRAIN_MOVING || 
            trains[trainIndex].state == TRAIN_WAITING) {
            activeTrainCount++;
        }
        else if (trains[trainIndex].state == TRAIN_ARRIVED) {
            deliveredCount++;
        }
        else if (trains[trainIndex].state == TRAIN_CRASHED) {
            crashedCount++;
        }
    }
    
    bool allTrainsFinished = (activeTrainCount == 0);
    bool maxTicksReached = (currentTick > 1000);
    
    if (allTrainsFinished) {
        cout << endl;
        cout << "=========================================" << endl;
        cout << "  SIMULATION COMPLETE!" << endl;
        cout << "=========================================" << endl;
        cout << "Total Ticks: " << currentTick << endl;
        cout << "Trains Delivered: " << deliveredCount << endl;
        cout << "Trains Crashed: " << crashedCount << endl;
        cout << "Switch Flips: " << switchFlips << endl;
        cout << "=========================================" << endl;
    }
    
    if (maxTicksReached) {
        cout << "\nWARNING: Maximum tick limit reached (1000 ticks)" << endl;
        cout << "Simulation ended to prevent infinite loops." << endl;
    }
    
    return allTrainsFinished || maxTicksReached;
}
