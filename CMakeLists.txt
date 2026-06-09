cmake_minimum_required(VERSION 4.2)
project(horodatage)

set(CMAKE_CXX_STANDARD 20)

add_executable(horodatage main.cpp
        TimeManager.h
        TimeManger.cpp
        FileManager.h
        FileManager.cpp
)
