// SensorDataSimulationAndProcessing.h: Standart sistem ekleme dosyaları için ekleme dosyası,
// veya projeye özgü ekleme dosyaları.

#pragma once

#include <iostream>
#include <sstream>

#include "Sensor.cpp"
#include "DataProcessor.cpp"

#include "console_utils.h"

std::mutex printMutex;

atomic<bool> isRunning{ true };
atomic<bool> isGenerate{ false };
bool printDataStatistics = true;

int processorPollingRate = 100; // 100 ms
int processorCollectSize = 5; // Collect 20 data per polling
int processorMaxPollingRate = 10000; // 10 seconds
int processorMaxCollectSize = 1000; // Collect 1000 data per polling

int configurationHeaderRow = 1;
int statisticsHeaderRow = 14;

int sensorStaticsStartRow = 3;
int sensorStaticsEndRow = 14;
int sensorStaticsStartCol = 1;

int processorStaticsStartRow = 3;
int processorStaticsEndRow = 14;
int processorStaticsStartCol = 50;

int rawStatisticsStartRow = 16;
int rawStatisticsEndRow = 27;
int rawStatisticsStartCol = 1;

int filteredStatisticsStartRow = 16;
int filteredStatisticsEndRow = 27;
int filteredStatisticsStartCol = 50;

int commandRow = 29;
int infoRow = 28;

template<typename dataType>
void displaySensorStatics(Sensor<dataType>& sensor);

template<typename dataType>
void displayProcessingStatics(DataProcessor<dataType>& processor);

template<typename dataType>
void displayStatistics(DataProcessor<dataType>& processor);

template <typename dataType>
void processCommand(string& command, Sensor<dataType>& sensor, DataProcessor<dataType>& processor);

template <typename dataType>
void commandThread(Sensor<dataType>& sensor, DataProcessor<dataType>& processor);

template <typename dataType>
void processingThread(Sensor<dataType>& sensor, DataProcessor<dataType>& processor);

