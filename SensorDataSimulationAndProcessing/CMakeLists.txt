# CMakeList.txt: SensorDataSimulationAndProcessing için CMake projesi, kaynağı ekleyin ve tanımlayın
# projeye özgü mantık burada bulunur.
#

# Kaynağı bu projenin yürütülebilir dosyasına ekleyin.
add_executable (SensorDataSimulationAndProcessing "SensorDataSimulationAndProcessing.cpp" "SensorDataSimulationAndProcessing.h" "global.h" "console_utils.h" "console_utils.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SensorDataSimulationAndProcessing PROPERTY CXX_STANDARD 20)
endif()

# TODO: Gerekirse testleri ve yükleme hedeflerini ekleyin.
