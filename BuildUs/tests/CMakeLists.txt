add_executable(buildtests 
    unittests.cpp
	catch.hpp
)

target_link_libraries(buildtests
    PRIVATE
    BuildUsUtils
)

target_include_directories(buildtests
    PRIVATE
    BuildUsUtils
)

file(COPY testsUtils DESTINATION ${CMAKE_BINARY_DIR}/tests)
