

using namespace std;

class DataProcessor {

	private:
		// DataProcessor attributes
		vector<double> rawData;
		int rawDataSize = 30;


	public:

		void test() { // Remove this function in the final version
			std::cout << "DataProcessor test" << std::endl;
		}

		int checkRawDataStatus() {

			// Check rawData empty, full, overfull or partially full
			if (this->rawData.empty()) {
				return 0; // Empty
			}
			else if (this->rawData.size() == this->rawDataSize) {
				return 1; // Full
			}
			else if (this->rawData.size() > this->rawDataSize) {
				return 2; // Overfull
			}
			else {
				return 3; // Partially full
			}
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

			int status = checkRawDataStatus();
			assignRawData(status, data);
		}

		void printRawData() {
			for (auto it = this->rawData.begin(); it != this->rawData.end(); ++it) {
				cout << *it << " ";
			}
			cout << endl;
		}

};