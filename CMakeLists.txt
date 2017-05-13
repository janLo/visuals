cmake_minimum_required(VERSION 3.2)
project(visuals)

file(GLOB FILES *.cpp *.hpp *.c *.h)

add_executable(visuals ${FILES})