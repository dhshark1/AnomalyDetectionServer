cmake_minimum_required(VERSION 3.15)
project(assignment)

set(CMAKE_C_STANDARD 99)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(ex6 anomaly_detection_util.cpp timeseries.cpp SimpleAnomalyDetector.cpp minCircle.cpp HybridAnomalyDetector.cpp CLI.cpp Server.cpp MainTrain.cpp)
