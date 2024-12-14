#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>
#include <ctime>

#define PERIODICALLY 1
#define DETERMINISTIC 1
#define UNBOUNDED 1
#define ASYNCHRONOUS 0
#define RANGE 0
#define RANDOM 0

using namespace std;

class Sensor {

private:

    mutex bufferMutex;

public:

    /* Default data attributes */
    int timing = PERIODICALLY;  // 0 - Asynchronous, 1 - Periodically
    int valueType = RANDOM;     // 0 - Random, 1 - Deterministic
    int limit = RANGE;          // 0 - Range, 1 - Unbounded

    int upperBound = 100;
    int lowerBound = 10;

    int period = 250;          // Default period of 1 second
    int minPeriod = 100;        // Minimum period of 100 milliseconds
    int maxPeriod = 2000;       // Maximum period of 2 seconds
    vector<double> dataBuffer;
    int dataBufferSize = 10;
    bool dataReady = false;

    bool isDataReady() {
		return this->dataReady;
	}

    void clearDataReady() {
		this->dataReady = false;
	}

    void setTiming(int timing) {
        if (timing == PERIODICALLY || timing == ASYNCHRONOUS) {
            this->timing = timing;
            return;
        }

        std::cout << "Invalid timing" << std::endl; // Remove this line in the final version
        return;
    }

    void setValueType(int type) {
        if (type == RANDOM || type == DETERMINISTIC) {
            this->valueType = type;
            return;
        }

        std::cout << "Invalid value type" << std::endl; // Remove this line in the final version
        return;
    }

    void setBounds(int lowerBound, int upperBound) {
        if (lowerBound < upperBound) {
            this->lowerBound = lowerBound;
            this->upperBound = upperBound;
            return;
        }

        std::cout << "Lower bound cannot be greater than upper bound" << std::endl; // Remove this line in the final version
        return;
    }

    double generateRandomDataPoint() {
        switch (this->limit) {
        case RANGE:
            return static_cast<double>((rand() % (this->upperBound - this->lowerBound + 1)) + this->lowerBound);
            break;

        case UNBOUNDED:
            return static_cast<double>(rand());
            break;

        default:
            break;
        }
        return 0.0; // Default return to avoid undefined behavior
    }

    double generateDeterministicDataPoint() {
        switch (this->limit) {
        case RANGE:
            return (rand() % (this->upperBound - this->lowerBound + 1)) + this->lowerBound;
            break;

        case UNBOUNDED:
            return rand();
            break;

        default:
            break;
        }
        return 0.0; // Default return to avoid undefined behavior
    }

    double generateDataPoint() {
        if (this->valueType == RANDOM) {
            return this->generateRandomDataPoint();
        }

        return this->generateDeterministicDataPoint();
    }

    void setBufferSize(int n) {

        if (n <= 0) {
            //cout << "Invalid buffer size" << endl; // Remove this line in the final version
            return;
        }
        else if(n != this->dataBufferSize) {
			//cout << "Buffer size changed to " << n << endl; // Remove this line in the final version
		}
		else {
			//cout << "Buffer size is already " << n << endl;     // Remove this line in the final version
            return;
		}

        lock_guard<mutex> lock(bufferMutex);
        // If the buffer size will decrease, delete the first (BufferSize - n) elements
        if (n < this->dataBuffer.size()) {
            this->dataBuffer.erase(this->dataBuffer.begin(), this->dataBuffer.begin() + (this->dataBuffer.size() - n));
        }


        this->dataBufferSize = n;
    }

    vector<double> collectData(int n) {

		setBufferSize(n);

        // Ensure the buffer is large enough before returning.
        if (dataBuffer.size() < n) {
            //cout << endl << "Insufficient data in the buffer!" << endl; // Remove this line in the final version
            return vector<double>(); // Return an empty vector if insufficient data.
        }
        return dataBuffer;
    }

    void generateTask() {

        while (true) {
            {
                lock_guard<mutex> lock(bufferMutex);

                double data = this->generateDataPoint();

                // Check if the buffer is full, if it is full delete first element and shift the rest to the left then add the new data point
                if (this->dataBuffer.size() == this->dataBufferSize) {
                    this->dataBuffer.erase(this->dataBuffer.begin());
                }

                this->dataBuffer.push_back(data);
                this->dataReady = true;
            }

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
