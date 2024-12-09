# CMakeList.txt: SensorDataSimulationAndProcessing için CMake projesi, kaynağı ekleyin ve tanımlayın
# projeye özgü mantık burada bulunur.
#

# Kaynağı bu projenin yürütülebilir dosyasına ekleyin.
add_executable (SensorDataSimulationAndProcessing "SensorDataSimulationAndProcessing.cpp" "SensorDataSimulationAndProcessing.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SensorDataSimulationAndProcessing PROPERTY CXX_STANDARD 20)
endif()

# TODO: Gerekirse testleri ve yükleme hedeflerini ekleyin.
