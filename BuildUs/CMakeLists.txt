cmake_minimum_required(VERSION 3.17)
project(BuildUs)

set(Boost_USE_STATIC_LIBS on)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.71.0 REQUIRED COMPONENTS iostreams filesystem)

add_subdirectory(BuildUsUtils)
add_subdirectory(yaml-cpp)
add_subdirectory(tests)

add_executable(BuildUs main.cpp)

target_link_libraries(BuildUs
    PRIVATE
	BuildUsUtils     
)

