# CMakeList.txt : CMake project for LibAdj, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("LibAdj")

include_directories(".")

# Add source to this project's executable.
add_executable (LibAdj "test/LibAdj.cpp" "LibAdj.h")

# TODO: Add tests and install targets if needed.
