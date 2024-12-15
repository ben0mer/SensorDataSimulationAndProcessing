# Steps to Download, Build and Running

### 1. Clone the Repository

Open a terminal or command prompt and run the following command to clone the repository:

```
git clone https://github.com/your-username/sensor-project.git
```

Alternatively, you can download the repository as a ZIP file from GitHub and extract it to a directory of your choice.

### 2. Open the Project in Visual Studio

- Launch Visual Studio 2022.

- From the main menu, select File > Open > CMake....

- Navigate to the directory containing the project and select the CMakeLists.txt file.

- Click Open to load the project.

### 3. Configure CMake

- When you open the project, Visual Studio will automatically detect the CMakeLists.txt file and start configuring the project. You can verify or adjust the settings as follows:

- Click on the CMake menu and select `Cache > Generate Cache for Sensor Project`.

- Ensure the default build configuration is set to Debug or Release.

### 4. Build the Project

- Select the desired build configuration (e.g., Debug or Release) from the dropdown menu at the top.

- Click `Build > Build All` or press Ctrl+Shift+B.

- This will compile the source files and generate the executable.

## Running the Project

### 1. Locate the Executable

After the build process completes, the executable file will be available in the build/<configuration> directory (e.g., build/Debug or build/Release).

### 2. Run the Application

- Open a terminal or command prompt.

- Navigate to the directory containing the executable.

- Run the application by typing:
```
SensorExecutable.exe
```

# Sensor Class

The `Sensor` class in C++ is a versatile template-based utility for simulating and managing sensor data generation. It supports both random and deterministic data generation, configurable bounds, and various timing mechanisms. This README outlines its functionality, attributes, and usage examples.

---

## Features

- **Random and Deterministic Data Generation**:
  - Generate data using random values or deterministic patterns (e.g., sawtooth wave).
  - Configurable limits (bounded or unbounded).
- **Timing Control**:
  - Periodic or asynchronous timing modes.
  - Adjustable period, minimum, and maximum timing intervals.
- **Data Management**:
  - Buffer management with adjustable size.
  - Thread-safe data generation and collection.
- **Real-Time Sensor Simulation**:
  - Continuous data generation in a background thread.
  - Ready-to-use methods to control simulation parameters dynamically.

---

## Class Template

```cpp
template<typename dataType>
class Sensor;
```

The class uses a template parameter (`dataType`) to support different data types, such as `int`, `float`, or `double`.

---

## Attributes

### Data Generation

- **`valueType`**: Specifies the type of data generation (`int`, default: RANDOM).
  - `0`: Random.
  - `1`: Deterministic (sawtooth wave).
- **`limit`**: Defines the range of data generation (`int`, default: RANGE).
  - `0`: Bounded.
  - `1`: Unbounded.
- **`upperBound` / `lowerBound`**: Bounds for data generation (`int`, defaults: 100 and 10, respectively).

### Timing Control

- **`timing`**: Sensor timing mode (`int`, default: PERIODICALLY).
  - `0`: Asynchronous.
  - `1`: Periodic.
- **`period`**: Interval between data generation in milliseconds (`int`, default: 250 ms).
- **`minPeriod` / `maxPeriod`**: Bounds for asynchronous timing intervals (`int`, defaults: 100 ms and 2000 ms).

### Data Buffer

- **`dataBufferSize`**: Maximum size of the data buffer (`int`, default: 5).
- **`dataBuffer`**: Stores generated data points (`std::vector<dataType>`).

---

## Public Methods

### Configuration

- **`void setTiming(int timing)`**: Sets the timing mode.
  - Valid values: `0` (Asynchronous), `1` (Periodic).
- **`void setValueType(int type)`**: Specifies the data generation type.
  - Valid values: `0` (Random), `1` (Deterministic).
- **`void setLimit(int value)`**: Configures data bounds.
  - Valid values: `0` (Ranged), `1` (Unbounded).
- **`void setBufferSize(int n)`**: Adjusts the buffer size.
- **`void setLowerBound(int value)`**: Sets the lower bound of generated data.
- **`void setUpperBound(int value)`**: Sets the upper bound of generated data.
- **`void setPeriod(int value)`**: Adjusts the period for periodic generation.
- **`void setMinPeriod(int value)`**: Configures the minimum period for asynchronous mode.
- **`void setMaxPeriod(int value)`**: Configures the maximum period for asynchronous mode.

### Data Collection

- **`std::vector<dataType> collectData(int n)`**: Retrieves `n` data points from the buffer.
  - Automatically adjusts buffer size if necessary.
  - Returns an empty vector if insufficient data is available.

### Control

- **`void startGeneration()`**: Starts data generation in a separate thread.
- **`bool isDataReady()`**: Checks if new data is available.
- **`void clearDataReady()`**: Resets the data readiness flag.

---

## Private Methods

- **`dataType generateRandomDataPoint()`**: Generates random data points.
- **`dataType generateDeterministicDataPoint()`**: Generates deterministic data points (e.g., sawtooth wave).
- **`dataType generateDataPoint()`**: Routes to the appropriate data generation method based on `valueType`.
- **`void generateTask()`**: Background task for continuous data generation.

---

## Example Usage

```cpp
#include "Sensor.h"
#include <iostream>
#include <vector>

int main() {
    Sensor<float> sensor;

    // Configure the sensor
    sensor.setValueType(1); // Deterministic
    sensor.setTiming(1);    // Periodic
    sensor.setLowerBound(0);
    sensor.setUpperBound(50);
    sensor.setPeriod(500);  // 500 ms interval

    // Start data generation
    sensor.startGeneration();

    // Simulate data collection after some time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    auto data = sensor.collectData(10);

    // Display collected data
    for (const auto& point : data) {
        std::cout << point << " ";
    }

    return 0;
}
```

---


# DataProcessor Class

The `DataProcessor` class is a C++ template designed for efficient data management and processing. It includes functionalities such as filtering, averaging, and analyzing raw data streams. This README provides an overview of its capabilities, attributes, and usage.

---

## Features

- **Data Filtering**:
  - Supports a moving average filter.
  - Easy customization of filter type and size.
- **Data Management**:
  - Handles raw data with adjustable size.
  - Automatically manages data overflow and partial updates.
- **Statistical Calculations**:
  - Calculate subset averages.
  - Retrieve minimum, maximum, and average values from data.

---

## Class Template

```cpp
template<typename dataType>
class DataProcessor;
```

The class accepts a `dataType` parameter to define the type of data being processed (e.g., `int`, `float`, or `double`).

---

## Public Methods and Attributes

### Attributes

- **rawDataSize**: Default size of the raw data buffer (`int`, default: 20).
- **maxRawDataSize**: Maximum allowed size for raw data (`int`, default: 600).
- **filterType**: Determines the type of filter to use (`int`, default: 1). 
  - `0`: No filter.
  - `1`: Moving average filter.
- **filterSize**: Size of the filter window (`int`, default: 5).

### Methods

#### Configuration Methods

- **`void setFilterType(int filterType)`**:
  - Sets the filter type. Valid values are `0` (No filter) and `1` (Moving average filter).
  - Outputs a success or error message.

- **`void setFilterSize(int filterSize)`**:
  - Configures the filter size. Must be greater than `0` and less than `rawDataSize`.
  - Outputs a success or error message.

- **`void setRawDataSize(int value)`**:
  - Adjusts the size of the raw data buffer.
  - Clears and reinitializes the raw data and filtered data buffers.
  - Valid range: `1` to `maxRawDataSize`.

#### Data Management Methods

- **`void inputData(vector<dataType> data)`**:
  - Inserts new data into the raw data buffer.
  - Automatically applies the configured filter.

- **`vector<dataType> getRawData()`**:
  - Returns the current raw data buffer.

- **`vector<double> getFilteredData()`**:
  - Returns the filtered data buffer.

- **`vector<double> getSubsetAverages()`**:
  - Returns calculated averages for subsets of the raw data.

#### Statistical Methods

- **`void calculateSubsetAverages(int subsetSize)`**:
  - Computes averages for subsets of the raw data with the specified size.

- **`template<typename T> double getMinValue(const vector<T>& data)`**:
  - Retrieves the minimum value from a dataset.

- **`template<typename T> double getMaxValue(const vector<T>& data)`**:
  - Retrieves the maximum value from a dataset.

- **`template<typename T> double calculateAverage(vector<T>& data)`**:
  - Computes the average of a dataset.

---

## Private Methods

### Core Functionalities

- **`void assignRawData(int status, vector<dataType> data)`**:
  - Handles data insertion based on the current status of the raw data buffer (empty, full, overfull, or partially full).

- **`void movingAverageFilter()`**:
  - Applies a moving average filter to the raw data buffer.

- **`void filterData()`**:
  - Applies the configured filter type to the raw data.

### Utility Methods

- **`int checkRawDataStatus()`**:
  - Checks the current state of the raw data buffer.
  - Returns:
    - `0`: Empty.
    - `1`: Full.
    - `2`: Overfull.
    - `3`: Partially full.

---

## Example Usage

```cpp
#include "DataProcessor.h"
#include <iostream>
#include <vector>

int main() {
    DataProcessor<int> processor;

    // Configure the processor
    processor.setRawDataSize(50);
    processor.setFilterType(1);
    processor.setFilterSize(5);

    // Input raw data
    processor.inputData({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // Retrieve processed data
    auto rawData = processor.getRawData();
    auto filteredData = processor.getFilteredData();

    // Display data
    std::cout << "Raw Data: ";
    for (const auto& val : rawData) std::cout << val << " ";

    std::cout << "\nFiltered Data: ";
    for (const auto& val : filteredData) std::cout << val << " ";

    return 0;
}
```

# Command Line Configuration

## Commands
### General Commands
- `start`: Starts the sensor data generation.
- `stop`: Stops the sensor data generation.

### Parameter Configuration
- Use the `set` command to configure properties.
- Format: `set property value`.

### Example Usage
To modify the "Number of Data Points":

```
set numberofdatapoints 30
```

### Adjustable Parameters
Below is a list of configurable parameters:
- **Sensor Parameters**:
  - `timing`: Set to `0` (Asynchronous) or `1` (Periodically).
  - `valuetype`: Set to `0` (Random) or `1` (Deterministic).
  - `limit`: Set to `0` (Range) or `1` (Unbounded).
  - `upperbound` and `lowerbound`: Define range limits.
  - `period`, `minperiod`, `maxperiod`: Control timing settings.
  - `databuffersize`: Adjust the size of the sensor data buffer.
- **Processor Parameters**:
  - `filtertype`: Set to `0` (No Filter) or `1` (Moving Average Filter).
  - `filtersize`: Size of the moving average filter.
  - `numberofdatapoints`: Total number of data points for processing.
  - `pollingrate`: Time interval (ms) for data polling.
  - `collectsize`: Number of data points collected per polling.
  - `printdata`: Set to `0` (Off) or `1` (On) for printing data.

### Customizing Data Type for Sensor
To modify the type of data generated by the sensor, you can adjust the sensorDataType in the `main` function. The `sensorDataType` can be set to `int`, `double`, or `float`. Each type influences the data format as follows:

```
using sensorDataType = int;   // Generates integer data.
using sensorDataType = float; // Generates floating-point data.
using sensorDataType = double; // Generates double-precision floating-point data.
```