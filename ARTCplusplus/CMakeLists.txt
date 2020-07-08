cmake_minimum_required(VERSION 3.15)
project(ARTCplusplus)

set(CMAKE_CXX_STANDARD 14)

add_executable(ARTCplusplus main.cpp auxiliary/model/Node.h auxiliary/model/Point.h auxiliary/model/Point.h fscs/art/FSCS_ART.h kdfc/art/KDFC_ART.h)