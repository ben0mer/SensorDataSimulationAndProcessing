#include <iomanip> // For formatting output
#include <algorithm> // For min_element and max_element

using namespace std;

template<typename dataType>
class DataProcessor {

	public:
		
		// DataProcessor attributes
		int rawDataSize = 20;
		int maxRawDataSize = 600;
		int filterType = 1; // 0: No filter, 1: Moving average filter
		int filterSize = 5; // Filter size for moving average filter

		void setFilterType(int filterType) {

			if (filterType >= 0 && filterType <= 1) {

				this->filterType = filterType;
				cout << "Data processor filter type successfully set.";
				return;
			}

			cout << "Invalid filter type. 0 - No filter, 1 - Moving average filter";
		}

		void setFilterSize(int filterSize) {

			if (filterSize > 0 && filterSize < rawDataSize) {

				this->filterSize = filterSize;
				cout << "Data processor filter size successfully set.";
				return;
			}

			cout << "Invalid filter size. Filter size must be greater than 0 and must be less than " << rawDataSize;
		}

		void setRawDataSize(int value) {

			if (value > 0 && value < this->maxRawDataSize) {

				this->rawDataSize = value;
				// Clear rawData and fill it with zeros rawDataSize times
				this->rawData.clear();
				this->rawData = vector<dataType>(value, 0);
				this->filteredData.clear();
				this->filteredData = vector<double>(value, 0);
				cout << "Data processor raw data size successfully set.";
				return;
			}

			cout << "Invalid raw data size. Raw data size must be greater than 0 and must be less than " << this->maxRawDataSize;
		}

		void inputData(vector<dataType> data) {

			assignRawData(checkRawDataStatus(), data);
			filterData();
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

		template<typename T>
		double getMinValue(const vector<T>& data) {

			if (data.empty()) return 0.0;
			return *min_element(data.begin(), data.end());
		}

		template<typename T>
		double getMaxValue(const vector<T>& data) {

			if (data.empty()) return 0.0;
			return *max_element(data.begin(), data.end());
		}

		template<typename T>
		double calculateAverage(vector<T>& data) {

			if (data.empty()) return 0.0;

			double sum = 0;
			for (double val : data) {

				sum += val;
			}
			return sum / data.size();
		}

		vector<dataType> getRawData() {

			return this->rawData;
		}

		vector<double> getFilteredData() {

			return this->filteredData;
		}

		vector<double> getSubsetAverages() {

			return this->subsetAverages;
		}

	private:

		// Fill rawData with zeros rawDataSize times
		vector<dataType> rawData = vector<dataType>(rawDataSize, 0);
		vector<double> filteredData = vector<double>(rawDataSize, 0); // This vector type should be double to store the average values
		vector<double> subsetAverages; // This vector type should be double to store the average values


		int checkRawDataStatus() {

			if (rawData.empty()) return 0; // Empty
			if (rawData.size() == rawDataSize) return 1; // Full
			if (rawData.size() > rawDataSize) return 2; // Overfull
			return 3; // Partially full
		}

		void assignRawData(int status, vector<dataType> data) {

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


		void movingAverageFilter() {

			if (this->rawData.size() < this->filterSize) return;

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

				this->filteredData.clear();
				this->filteredData = vector<double>(this->rawData.begin(), this->rawData.end());
				break;

			case 1: // Moving average filter

				movingAverageFilter();
				break;

			case 2: // Different filter types can be added in the future

				break;

			default:
				break;
			}

		}

};