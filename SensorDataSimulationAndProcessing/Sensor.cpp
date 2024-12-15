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

template<typename dataType>
class Sensor {

    private:

        mutex bufferMutex;
        vector<dataType> dataBuffer;
        bool dataReady = false;

        dataType generateRandomDataPoint() {

            switch (this->limit) {

            case RANGE:

                return this->lowerBound + static_cast <dataType> (rand()) / (static_cast <dataType> (RAND_MAX / (this->upperBound - (this->lowerBound))));
                break;

            case UNBOUNDED:

                return static_cast<dataType>(rand()) / static_cast<dataType>(RAND_MAX) * static_cast<dataType>(2e6) - static_cast<dataType>(1e6);
                break;

            default:

                return 0.0; // Default return to avoid undefined behavior
            }
        }

        // Generate sawtooth wave data
        dataType generateDeterministicDataPoint() {

            static int step = 0;
            int period = 100;

            switch (this->limit) {

            case RANGE:

                return this->lowerBound +
                    static_cast<dataType>((step++ % period) / static_cast<double>(period)) *
                    (this->upperBound - this->lowerBound);
                break;

            case UNBOUNDED:

                return static_cast<dataType>((step++ % period) / static_cast<double>(period));
                break;

            default:

                return 0.0;
            }
        }

        dataType generateDataPoint() {

            if (this->valueType == RANDOM) {

                return this->generateRandomDataPoint();
            }

            return this->generateDeterministicDataPoint();
        }


        void generateTask() {

            while (true) {

                {
                    lock_guard<mutex> lock(bufferMutex);

                    dataType data = this->generateDataPoint();

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
        int dataBufferSize = 5;

        void setBufferSize(int n) {

            if (n > 0 && n != this->dataBufferSize) {

                lock_guard<mutex> lock(bufferMutex);
                // If the buffer size will decrease, delete the first (BufferSize - n) elements
                if (n < this->dataBuffer.size()) {

                    this->dataBuffer.erase(this->dataBuffer.begin(), this->dataBuffer.begin() + (this->dataBuffer.size() - n));
                }

                this->dataBufferSize = n;
                return;
            }
        }

        vector<dataType> collectData(int n) {

		    setBufferSize(n);

            // Ensure the buffer is large enough before returning.
            if (dataBuffer.size() < n) {

                return vector<dataType>(); // Return an empty vector if insufficient data.
            }

            return dataBuffer;
        }

        void startGeneration() {

            thread t(&Sensor::generateTask, this);
            t.detach(); // Detach the thread from the main thread
        }

        // Get dataType function
        dataType getDataType() {

            return dataType();
        }

        bool isDataReady() {

            return this->dataReady;
        }

        void clearDataReady() {

            this->dataReady = false;
        }

        void setTiming(int timing) {

            if (timing == PERIODICALLY || timing == ASYNCHRONOUS) {

                this->timing = timing;
                cout << "Sensor timing successfully set.";
                return;
            }

            cout << "Invalid timing, 0 - Asynchronous, 1 - Periodically"; // Remove this line in the final version
        }

        void setValueType(int type) {

            if (type == RANDOM || type == DETERMINISTIC) {

                this->valueType = type;
                cout << "Sensor value type successfully set.\n";
                return;
            }

            cout << "Invalid value type, 0 - Random, 1 - Deterministic"; // Remove this line in the final version
        }

        void setLimit(int value) {

            if (value == RANGE || value == UNBOUNDED) {

                this->limit = value;
                cout << "Sensor value limit successfully set.\n";
                return;
            }

            cout << "Invalid value limit, 0 - Range, 1 - Unbounded"; // Remove this line in the final version
        }

        void setLowerBound(int value) {

            if (value < this->upperBound) {

                this->lowerBound = value;
                cout << "Lower bound successfully set.\n";
                return;
            }

            cout << "Invalid lower bound value. Lower bound cannot be greater than upper bound.\n";
        }

        void setUpperBound(int value) {

            if (value > this->lowerBound) {

                this->upperBound = value;
                cout << "Upper bound successfully set.\n";
                return;
            }

            cout << "Invalid upper bound value. Upper bound cannot be less than lower bound.\n";
        }

        void setPeriod(int value) {

            if (value > 0) {

                this->period = value;
                cout << "Period successfully set.\n";
                return;
            }

            cout << "Invalid period value. Period must be greater than 0.\n";
        }

        void setMinPeriod(int value) {

            if (value > 0) {

                this->minPeriod = value;
                cout << "Minimum period successfully set.\n";
                return;
            }

            cout << "Invalid minimum period value. Minimum period must be greater than 0.\n";
        }

        void setMaxPeriod(int value) {

            if (value > 0 && value > this->minPeriod) {

                this->maxPeriod = value;
                cout << "Maximum period successfully set.\n";
                return;
            }

            cout << "Invalid maximum period value. Maximum period must be greater than 0 and greater than minimum period.\n";
        }
};
