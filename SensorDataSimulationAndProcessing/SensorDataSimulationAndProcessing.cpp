// SensorDataSimulationAndProcessing.cpp: Uygulamanın giriş noktasını tanımlar.
//

#include "SensorDataSimulationAndProcessing.h"
#include "Sensor.cpp"

using namespace std;

int main()
{
	Sensor sensor;
	sensor.startGeneration();
	for (;;) {
		//sensor.test();
		this_thread::sleep_for(chrono::seconds(2));
		sensor.collectData(3);
		this_thread::sleep_for(chrono::seconds(2));
		sensor.collectData(7);

	}
	return 0;
}
