// SensorDataSimulationAndProcessing.cpp: Uygulamanın giriş noktasını tanımlar.
//
#include <iostream>
#include <sstream>

#include "SensorDataSimulationAndProcessing.h"
#include "Sensor.cpp"
#include "DataProcessor.cpp"
#include "global.h"
#include "console_utils.h"

using namespace std;

atomic<bool> isRunning{ true };
atomic<bool> isGenerate{ false };

void displaySensorStatics(Sensor& sensor) {
	std::ostringstream stats;
    stats << "Sensor Configuration:\n";
    stats << "- Timing: " << (sensor.timing == 0 ? "Asynchronous" : "Periodically") << endl;
    stats << "- Value Type: " << (sensor.valueType == 0 ? "Random" : "Deterministic") << endl;
    stats << "- Limit: " << (sensor.limit == 0 ? "Range" : "Unbounded") << endl;
    stats << "- Upper Bound: " << sensor.upperBound << endl;
    stats << "- Lower Bound: " << sensor.lowerBound << endl;
    stats << "- Period: " << sensor.period << endl;
    stats << "- Min Period: " << sensor.minPeriod << endl;
    stats << "- Max Period: " << sensor.maxPeriod << endl;
    stats << "- Data Buffer Size: " << sensor.dataBufferSize << endl;

	printInRegion(1, 1, 10, stats.str()); // Region 1: Rows 1-10
}

void displayProcessingStatics(DataProcessor& processor) {
	std::ostringstream stats;
	stats << "Data Processor Configuration:\n";
	stats << "- Filter Type: " << processor.filterType << endl;
	stats << "- Filter Size: " << processor.filterSize << endl;

	printInRegion(25, 1, 20, stats.str()); // Region 1: Rows 1-10
}

void processCommand(string& command, Sensor& sensor, DataProcessor& processor) {
    clearLine(23);
	istringstream iss(command);
	string action, property;
	iss >> action;

    if (action == "set") {
        iss >> property;
        if (property == "timing") {
            double value;
            iss >> value;
            //sensor.configureTimingInterval(value);
            cout << "Sensor timing interval set to " << value << " seconds.\n";
        }
        else if (property == "valueType") {
            string value;
            iss >> ws;
            getline(iss, value);
            //sensor.configureValueType(value);
            cout << "Sensor value type set to " << value << ".\n";
        }
        else if (property == "filterType") {
            int value;
            iss >> value;
            //processor.configureFilterType(value);
            cout << "DataProcessor filter type set to " << value << ".\n";
        }
        else if (property == "filterSize") {
            int value;
            iss >> value;
            //processor.configureFilterSize(value);
            cout << "DataProcessor filter size set to " << value << ".\n";
        }
        else {
            cout << "Unknown property: " << property << "\n";
        }
        
    }
    else if (action == "start") {
        //sensor.startGenerate();
        sensor.startGeneration();
        isGenerate = true;
        cout << "start triggered \n ";
    }
    else if (action == "stop") {
        //sensor.stopGenerate();
        isGenerate = false;
        cout << "stop triggered \n ";
    }
    else if (action == "show") {
        cout << "show triggered \n";
        //sensor.printSettings();
        //processor.printSettings();
    }
    else {
        cout << "Unknown command: " << action << "\n";
    }

    clearLine(22);
    displaySensorStatics(sensor);
    displayProcessingStatics(processor);
}

void commandThread(Sensor& sensor, DataProcessor& processor, int commandRow) {
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
                moveCursorTo(commandRow + 1, 1); // Move below command line for the message
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

void displayStatistics(DataProcessor& processor) {

    std::ostringstream stats;
    stats << "Signal Statistics:\n";
    //stats << "Min Value: " << processor.getMinValue() << "\n";
    //stats << "Max Value: " << processor.getMaxValue() << "\n";
    stats << "Average: " << processor.calculateAverage() << "\n";

    printInRegion(1, 11, 13, stats.str()); // Region 1: Rows 1-10

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //resetCursorToCommandRegion(22);

}

void processingThread(Sensor& sensor, DataProcessor& processor) {
    while (isRunning) {
        if (isGenerate && sensor.isDataReady()) {
            vector<double> data = sensor.collectData(20);
            if (!data.empty()) {
                processor.inputData(data);
                displayStatistics(processor);
                //processor.printRawData();
            }
            sensor.clearDataReady();
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}


int main()
{
	Sensor sensor;
	DataProcessor processor;
    displaySensorStatics(sensor);
    displayProcessingStatics(processor);

    thread command(commandThread, ref(sensor), ref(processor), 22);
    thread processing(processingThread, ref(sensor), ref(processor));

    command.join();
    processing.join();

	return 0;
}
