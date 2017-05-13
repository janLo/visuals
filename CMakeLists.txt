cmake_minimum_required(VERSION 3.0)
project(visuals)

file(GLOB FILES *.cpp *.hpp *.c *.h)

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_executable(visuals ${FILES})