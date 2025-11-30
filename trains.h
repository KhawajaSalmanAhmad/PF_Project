#ifndef TRAINS_H
#define TRAINS_H

void spawnTrainsForTick();

void determineAllRoutes();
bool determineNextPosition(int trainIdx, int& nextX, int& nextY, int& nextDir);
int getNextDirection(int x, int y, char tile, int currentDir);
int getSmartDirectionAtCrossing(int trainIdx, int x, int y, int currentDir);

void moveAllTrains();

void detectCollisions();
int calculateDistance(int trainIdx, int x, int y);

void checkArrivals();

#endif
