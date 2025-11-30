#ifndef IO_H
#define IO_H

bool loadLevelFile(const char* filename);

void initializeLogFiles();

void logTrainTrace();

void logSwitchState();

void logSignalState();

void writeMetrics();

#endif
