// SensorDataSimulationAndProcessing.cpp: Uygulamanın giriş noktasını tanımlar.
//
#include <iostream>


#include "SensorDataSimulationAndProcessing.h"
#include "Sensor.cpp"
#include "DataProcessor.cpp"

using namespace std;

int main()
{
	Sensor sensor;
	DataProcessor processor;

	sensor.startGeneration();
	for (;;) {
		
		if (sensor.isDataReady()) {
			vector<double> data = sensor.collectData(8);
			if (!data.empty()) {
				processor.inputData(data);
				processor.printRawData();
			}
			sensor.clearDataReady();
			
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	return 0;
}
