# cmake_minimum_required(VERSION <specify CMake version here>)
project(advance_programing_project1)

set(CMAKE_CXX_STANDARD 14)

add_executable(advance_programing_project1 SimpleAnomalyDetector.cpp timeseries.cpp anomaly_detection_util.cpp MainTrain.cpp)