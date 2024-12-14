#include <iomanip> // For formatting output
#include <algorithm> // For min_element and max_element
#include "global.h"

using namespace std;

class DataProcessor {


	public:
		
		// DataProcessor attributes
		int rawDataSize = 10;
		int filterType = 1; // 0: No filter, 1: Moving average filter, 2: Median filter
		int filterSize = 5; // Filter size for moving average filter and median filter

		int checkRawDataStatus() {

			if (rawData.empty()) return 0; // Empty
			if (rawData.size() == rawDataSize) return 1; // Full
			if (rawData.size() > rawDataSize) return 2; // Overfull
			return 3; // Partially full
		}

		void assignRawData(int status, vector<double> data) {
			switch (status)
			{
			case 0: // If rawData is empty

				if (data.size() <= this->rawDataSize) {
					// If data size is smaller than or equal to rawDataSize, insert the data to rawData
					this->rawData.insert(this->rawData.end(), data.begin(), data.end());
					return;
				}
				else {
					// If data size is larger than rawDataSize, insert the last rawDataSize elements of the data to rawData
					this->rawData.insert(this->rawData.end(), data.begin() + (data.size() - this->rawDataSize), data.end());
					return;
				}
				break;

			case 1: // If rawData is full

				if (data.size() < rawDataSize) {

					// If data size is smaller than rawDataSize, erase the first rawDataSize - data.size() elements of rawData and insert the data to the end
					this->rawData.erase(this->rawData.end() - (data.size() - 1), this->rawData.end());
					this->rawData.insert(this->rawData.end(), data.begin(), data.end());
					this->rawData.erase(this->rawData.begin());
					return;

				}
				else if (data.size() == rawDataSize) {

					// If data size is equal to rawDataSize, replace the rawData with the data
					this->rawData = data;
					return;
				}
				else {

					this->rawData.clear();
					this->rawData.insert(this->rawData.end(), data.begin() + (data.size() - this->rawDataSize), data.end());
					return;
				}
				break;

			case 2: // If rawData is overfull

				// If rawData is overfull, erase the first rawData.size() - rawDataSize elements of rawData and call assignRawData again
				this->rawData.erase(this->rawData.begin(), this->rawData.begin() + (this->rawData.size() - this->rawDataSize));
				assignRawData(checkRawDataStatus(), data);
				break;

			case 3: // If rawData is partially full

				// If rawData is partially full, insert the data to rawData
				if ((this->rawData.size() + data.size()) <= this->rawDataSize) {

					this->rawData.insert(this->rawData.end(), data.begin(), data.end());
				}
				else {

					// erase first rawDataSize - (rawData.size() - data.size()) elements of rawData and insert the data to the end
					this->rawData.erase(this->rawData.begin(), this->rawData.begin() + (this->rawData.size() + data.size() - this->rawDataSize));
					this->rawData.insert(this->rawData.end(), data.begin(), data.end());
				}
				break;

			default:
				break;
			}
		}

		void inputData(vector<double> data) {

			assignRawData(checkRawDataStatus(), data);
			filterData();
		}

		void movingAverageFilter() {

			if (rawData.size() < filterSize) return;

			double sum = 0;

			// Get last filterSize elements of rawData, sum them and divide by filterSize
			for (int i = this->rawData.size() - this->filterSize; i < this->rawData.size(); i++) {
				sum += this->rawData[i];
			}

			this->filteredData.push_back(sum / this->filterSize);
			this->filteredData.erase(this->filteredData.begin());

		}

		void filterData() {
			switch (this->filterType)
			{
			case 0: // No filter
				this->filteredData = this->rawData;
				break;

			case 1: // Moving average filter
				movingAverageFilter();
				break;

			case 2: // Median filter
				//medianFilter(this->rawData, this->filterSize);
				break;

			default:
				break;
			}

		}

		double calculateAverage() {

			if (rawData.empty()) return 0.0;

			double sum = 0;
			for (double val : rawData) {
				sum += val;
			}
			return sum / rawData.size();
		}

		void calculateSubsetAverages(int subsetSize) {
			this->subsetAverages.clear();

			if (subsetSize <= 0 || rawData.size() < subsetSize) return; // Return if subsetSize is invalid or rawData is smaller than subsetSize

			for (size_t i = 0; i <= rawData.size() - subsetSize; i += subsetSize) {
				double sum = 0;
				for (size_t j = i; j < i + subsetSize; j++) {
					sum += rawData[j];
				}
				this->subsetAverages.push_back(sum / subsetSize);
			}
		}

		double getMinValue(const vector<double>& data) {
			if (data.empty()) return 0.0;
			return *min_element(data.begin(), data.end());
		}

		double getMaxValue(const vector<double>& data) {
			if (data.empty()) return 0.0;
			return *max_element(data.begin(), data.end());
		}

		vector<double> getRawData() {
			return this->rawData;
		}

		vector<double> getFilteredData() {
			return this->filteredData;
		}

		vector<double> getSubsetAverages() {
			return this->subsetAverages;
		}

		void printSubsetAverages() {
			cout << "Subset Averages: ";
			for (double val : this->subsetAverages) {
				cout << val << " ";
			}
			cout << endl;
		}

		void printRawData() { // Remove this function in the final version
			lock_guard<mutex> lock(printMutex);
			cout << "\033[s";                    // Save cursor position
			cout << "\033[H";                    // Move to top of the console
			cout << "\033[2KProcessed Data: \n";   // Clear the line and write

			for (auto it = this->rawData.begin(); it != this->rawData.end(); ++it) {
				cout << *it << ", ";
			}
			cout << endl;

			for (auto it = this->filteredData.begin(); it != this->filteredData.end(); ++it) {
				cout << *it << ", ";
			}
			cout << endl;

			cout << "\033[u";                    // Restore cursor position

			cout.flush();
		}
	private:


		// Fill rawData with zeros rawDataSize times
		vector<double> rawData = vector<double>(rawDataSize, 0);
		vector<double> filteredData = vector<double>(rawDataSize, 0);
		vector<double> subsetAverages;

};