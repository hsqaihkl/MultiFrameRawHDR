cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MultiFrameRawHDR LANGUAGES CXX C)

find_package(OpenCV REQUIRED)
add_executable(${CMAKE_PROJECT_NAME} demo.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
