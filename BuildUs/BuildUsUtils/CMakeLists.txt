add_library(BuildUsUtils
	buildus.cpp
	buildus.h
)

target_link_libraries(BuildUsUtils
    PUBLIC
	yaml-cpp
	${Boost_LIBRARIES}
)

target_include_directories(BuildUsUtils
    PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		${Boost_INCLUDE_DIRS}
)