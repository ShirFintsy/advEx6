cmake_minimum_required(VERSION 3.21)
project(advEx6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(advEx6 anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h CLI.cpp CLI.h
        commands.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h MainTrain.cpp minCircle.cpp minCircle.h
        Server.cpp Server.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h)
