cmake_minimum_required (VERSION 2.6)
project (WeaponSys++)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (WeaponSys++ main.cpp release.cpp)
