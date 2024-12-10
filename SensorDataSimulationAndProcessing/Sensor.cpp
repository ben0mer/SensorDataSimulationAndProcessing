
#include <vector>
#include <thread>

#define PERIODICALLY 1
#define DETERMINISTIC 1
#define UNBOUNDED 1
#define ASYNCHRONOUS 0
#define RANGE 0
#define RANDOM 0


using namespace std;
class Sensor {

	private:

		/* Default data attributes */
		int timing = PERIODICALLY;	// 0 - Asynchronous, 1 - Periodically
		int valueType = RANDOM;		// 0 - Random, 1 - Deterministic
		int limit = RANGE;			// 0 - Range, 1 - Unbounded

		int upperBound = 100;
		int lowerBound = 0;

		int period = 100;			// Default period of 1 second
		int minPeriod = 100;		// Minimum period of 100 milliseconds
		int maxPeriod = 2000;		// Maximum period of 2 seconds
		vector<double> dataBuffer;
		int dataBufferSize = 10;
		bool dataReady = false;

	public:

		void test() {
			std::cout << "Sensor test" << std::endl;
		}

		void setTiming(int timing) {
			if (timing == PERIODICALLY || timing == ASYNCHRONOUS) {
				this->timing = timing;
				return;
			}

			std::cout << "Invalid timing" << std::endl;
			return;
		}

		void setValueType(int type) {
			if (type == RANDOM || type == DETERMINISTIC) {
				this->valueType = type;
				return;
			}

			std::cout << "Invalid value type" << std::endl;
			return;
		}

		void setBounds(int lowerBound, int upperBound) {
			if (lowerBound < upperBound) {
				this->lowerBound = lowerBound;
				this->upperBound = upperBound;
				return;
			}

			std::cout << "Lower bound cannot be greater than upper bound" << std::endl;
			return;
		}

		double generateRandomDataPoint() {

			switch (this->limit)
			{
				case RANGE:
					return static_cast<double>((rand() % (this->upperBound - this->lowerBound + 1)) + this->lowerBound);
					break;

				case UNBOUNDED:
					return rand();
					break;

				default:
					break;
			}
		}

		double generateDeterministicDataPoint() {

			// Implement deterministic data generation logic here
			switch (this->limit)
			{
				case RANGE:
					return (rand() % (this->upperBound - this->lowerBound + 1)) + this->lowerBound;
					break;

				case UNBOUNDED:
					return rand();
					break;

				default:
					break;
			}
		}

		double generateDataPoint(){
			if (this->valueType == RANDOM) {
				return this->generateRandomDataPoint();
			}

			return this->generateDeterministicDataPoint();
		}

		/*
		vector<double> collectData(int n) {
			vector<double> data;
			for (int i = 0; i < n; i++) {
				data.push_back(this->generateDataPoint());
				cout << data[i] << endl; // Print the data point for debugging purposes
				this_thread::sleep_for(chrono::milliseconds(this->period));

			}
			return data;
		}
		*/

		void setBufferSize(int n) {

			// Check if the buffer size is valid
			if (n <= 0) {
				cout << "Invalid buffer size" << endl;
				return;
			}

			// Ýf the buffer size will decrease, delete the first (BufferSize - n) elements
			if (n < this->dataBufferSize) {
				this->dataBuffer.erase(this->dataBuffer.begin(), this->dataBuffer.begin() + (this->dataBufferSize - n));
			}
			this->dataBufferSize = n;
		}

		vector<double> collectData(int n) {
			setBufferSize(n);

			// If the buffer is not full return an empty vector since the buffer is not full. This condition maybe add in the future
			return this->dataBuffer;
		}

		void generateTask() {

			for (;;) {

				// Print the data buffer for debugging purposes
				for (auto it = this->dataBuffer.begin(); it != this->dataBuffer.end(); ++it) {
					cout << *it << " - ";
				}
				cout << endl;

				double data = this->generateDataPoint();

				// Check if the buffer is full, if it is full delete first element and shift the rest to the left then add the new data point
				if (this->dataBuffer.size() == this->dataBufferSize) {
					this->dataBuffer.erase(this->dataBuffer.begin());
				}

				this->dataBuffer.push_back(data);
				this->dataReady = true;

				// Sleep for the period
				if (this->timing == PERIODICALLY) {
					this_thread::sleep_for(chrono::milliseconds(this->period));
				}
				else {
					this_thread::sleep_for(chrono::milliseconds(rand() % (this->maxPeriod - this->minPeriod + 1) + this->minPeriod));
				}
			}
		}

		void startGeneration() {
			thread t(&Sensor::generateTask, this);
			t.detach(); // Detach the thread from the main thread
		}


	};