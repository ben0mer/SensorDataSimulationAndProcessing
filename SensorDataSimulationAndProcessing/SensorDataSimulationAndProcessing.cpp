
#include "SensorDataSimulationAndProcessing.h"

using namespace std;


int main()
{
	using sensorDataType = float; // This type can be int, double and float.

	Sensor<sensorDataType> sensor;
	DataProcessor<sensorDataType> processor;
    displaySensorStatics(sensor);
    displayProcessingStatics(processor);

    thread command(commandThread<sensorDataType>, ref(sensor), ref(processor));
    thread processing(processingThread<sensorDataType>, ref(sensor), ref(processor));

    command.join();
    processing.join();

	return 0;
}

template <typename dataType>
void displaySensorStatics(Sensor<dataType>& sensor) {

    std::ostringstream stats;

    stats << "SENSOR CONFIGURATION:\n";
    stats << "|- Timing: " << (sensor.timing == 0 ? "Asynchronous" : "Periodically") << endl;
    stats << "|- Period: " << sensor.period << endl;
    stats << "|__ Min Period: " << sensor.minPeriod << endl;
    stats << "|__ Max Period: " << sensor.maxPeriod << endl;
    stats << "|- Value Type: " << (sensor.valueType == 0 ? "Random" : "Deterministic") << endl;
    stats << "|- Limit: " << (sensor.limit == 0 ? "Range" : "Unbounded") << endl;
    stats << "|__ Upper Bound: " << sensor.upperBound << endl;
    stats << "|__ Lower Bound: " << sensor.lowerBound << endl;
    stats << "|- Data Buffer Size: " << sensor.dataBufferSize << endl;

    printInRegion(sensorStaticsStartCol, sensorStaticsStartRow, sensorStaticsEndRow, stats.str()); // Region 1: Rows 1-10
}

template <typename dataType>
void displayProcessingStatics(DataProcessor<dataType>& processor) {

    std::ostringstream stats;

    stats << "         ################ -- SENSOR AND DATA PROCESSOR CONFIGURATION -- ################\n";
    printInRegion(1, configurationHeaderRow, configurationHeaderRow + 1, stats.str()); // Region 1: Rows 1-10

    stats.str("");

    stats << "DATA PROCESSOR CONFIGURATION:\n";
    stats << "|- Filter Type: " << (processor.filterType == 0 ? "No Filter" : "Moving Avarage Filter") << endl;
    stats << "|- Filter Size: " << processor.filterSize << endl;
    stats << "|- Number of Data Points: " << processor.rawDataSize << endl;
    stats << "|- Polling Rate: " << processorPollingRate << " ms" << endl;
    stats << "|- Collect Size: " << processorCollectSize << " data per polling" << endl;
    stats << "|- Print Data: " << (printDataStatistics ? "True" : "False") << endl;

    printInRegion(processorStaticsStartCol, processorStaticsStartRow, processorStaticsEndRow, stats.str()); // Region 1: Rows 1-10
}

template <typename dataType>
void displayStatistics(DataProcessor<dataType>& processor) {

    std::ostringstream stats;

    stats << "         ################ -------- PROCESSED SIGNAL STATISTICS -------- ################\n";
    printInRegion(1, statisticsHeaderRow, statisticsHeaderRow + 1, stats.str());


    vector<dataType> dataRaw = processor.getRawData();
    vector<double> dataFiltered = processor.getFilteredData();

    stats.str("");
    stats << "RAW DATA STATISTICS:\n";
    stats << "|- Number of Data Points: " << dataRaw.size() << "\n";
    stats << "|- Min Value: " << processor.getMinValue(dataRaw) << "\n";
    stats << "|- Max Value: " << processor.getMaxValue(dataRaw) << "\n";
    stats << "|- Average: " << processor.calculateAverage(dataRaw) << "\n";

    printInRegion(rawStatisticsStartCol, rawStatisticsStartRow, rawStatisticsEndRow, stats.str());

    stats.str("");
    stats << "FILTERED DATA STATISTICS:\n";
    stats << "|- Number of Data Points: " << dataFiltered.size() << "\n";
    stats << "|- Min Value: " << processor.getMinValue(dataFiltered) << "\n";
    stats << "|- Max Value: " << processor.getMaxValue(dataFiltered) << "\n";
    stats << "|- Average: " << processor.calculateAverage(dataFiltered) << "\n";

    printInRegion(filteredStatisticsStartCol, filteredStatisticsStartRow, filteredStatisticsEndRow, stats.str());

    if (!printDataStatistics) {

        return;
    }

    stats.str("");
    stats << "FILTERED DATA: ";
    for (auto it = dataFiltered.begin(); it != dataFiltered.end(); ++it) {

        stats << *it << " ";
    }
    stats << "\n\n";

    stats << "RAW DATA: ";
    for (auto it = dataRaw.begin(); it != dataRaw.end(); ++it) {

        stats << *it << " ";
    }

    printInRegion(1, rawStatisticsEndRow - 4, rawStatisticsEndRow, stats.str());

}

template <typename dataType>
void processCommand(string& command, Sensor<dataType>& sensor, DataProcessor<dataType>& processor) {

    clearLine(infoRow);
    istringstream iss(command);
    string action, property;
    iss >> action;

    if (action == "set") {

        int value;
        iss >> property;
        if (property == "timing") {
            iss >> value;
            sensor.setTiming(value);
        }
        else if (property == "valuetype") {
            iss >> value;
            sensor.setValueType(value);
        }
        else if (property == "limit") {
            iss >> value;
            sensor.setLimit(value);
        }
        else if (property == "upperbound") {
            iss >> value;
            sensor.setUpperBound(value);
        }
        else if (property == "lowerbound") {
            iss >> value;
            sensor.setLowerBound(value);
        }
        else if (property == "period") {
            iss >> value;
            sensor.setPeriod(value);
        }
        else if (property == "minperiod") {
            iss >> value;
            sensor.setMinPeriod(value);
        }
        else if (property == "maxperiod") {
            iss >> value;
            sensor.setMaxPeriod(value);
        }
        else if (property == "databuffersize") {
            iss >> value;
            sensor.setBufferSize(value);
        }
        else if (property == "filtertype") {
            iss >> value;
            processor.setFilterType(value);
        }
        else if (property == "filtersize") {
            iss >> value;
            processor.setFilterSize(value);
        }
        else if (property == "numberofdatapoints") {
            iss >> value;
            processor.setRawDataSize(value);
        }
        else if (property == "pollingrate") {
            iss >> value;
            if (value > 0 && value < processorMaxPollingRate) {

                processorPollingRate = value;
                cout << "Polling rate successfully set.\n";
            }
            else {

                cout << "Invalid polling rate. Polling rate must be greater than 0 and less than " << processorMaxPollingRate;
            }
        }
        else if (property == "collectsize") {
            iss >> value;
            if (value > 0 && value < processorMaxCollectSize) {

                processorCollectSize = value;
                cout << "Collect size successfully set.\n";
            }
            else {

                cout << "Invalid collect size. Collect size must be greater than 0 and less than " << processorMaxCollectSize;

            }
        }
        else if (property == "printdata") {

            iss >> value;
            if (value == 0) {

                printDataStatistics = false;
                cout << "Print data statistics turned off.\n";
            }
            else if (value == 1) {

                printDataStatistics = true;
                cout << "Print data statistics turned on.\n";
            }
            else {

                cout << "Invalid print data value. 0 - Turn off, 1 - Turn on.\n";
            }
        }
        else {

            cout << "Unknown property: " << property << "\n";
        }

    }
    else if (action == "start") {

        sensor.startGeneration();
        isGenerate = true;
        cout << "Sensor simulation running. \n ";
    }
    else if (action == "stop") {

        isGenerate = false;
        cout << "Sensor simulation stopped. \n ";
    }
    else if (action == "help") {

        cout << "HELP \n";
    }
    else {

        cout << "Unknown command: " << action << "\n";
    }

    clearLine(commandRow);
    displaySensorStatics(sensor);
    displayProcessingStatics(processor);
}

template <typename dataType>
void commandThread(Sensor<dataType>& sensor, DataProcessor<dataType>& processor) {

    std::string command;

    while (isRunning) {

        // Reset cursor to the command row for user input
        resetCursorToCommandRegion(commandRow);

        // Read user input
        std::getline(std::cin, command);

        if (command == "exit") {
            isRunning = false;

            // Use mutex to safely print the exit message
            {
                std::lock_guard<std::mutex> lock(printMutex);
                moveCursorTo(infoRow, 1); // Move below command line for the message
                std::cout << "Exiting program.\n";
            }
            break;
        }

        // Process the user command
        processCommand(command, sensor, processor);

        // After processing, ensure cursor returns to the command line
        resetCursorToCommandRegion(commandRow);
    }
}

template <typename dataType>
void processingThread(Sensor<dataType>& sensor, DataProcessor<dataType>& processor) {

    while (isRunning) {

        if (isGenerate && sensor.isDataReady()) {

            vector<dataType> data = sensor.collectData(processorCollectSize);
            if (!data.empty()) {

                processor.inputData(data);
                displayStatistics(processor);
            }

            sensor.clearDataReady();
        }

        this_thread::sleep_for(chrono::milliseconds(processorPollingRate));
    }
}

