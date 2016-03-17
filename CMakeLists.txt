cmake_minimum_required(VERSION 3.3)
project(seed11)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(SOURCE_FILES
	src/seed11_system_agnostic.cpp
	src/deterministic_unsafe_seed_device.cpp)

if(${WIN32})
	set(SOURCE_FILES ${SOURCE_FILES} src/seed11_windows.cpp)
else(${WIN32})
	set(SOURCE_FILES ${SOURCE_FILES} src/seed11_linux.cpp)
endif(${WIN32})

include_directories(include)

add_library(seed11 STATIC ${SOURCE_FILES})
target_compile_features(seed11 PUBLIC cxx_deleted_functions cxx_rvalue_references cxx_defaulted_functions cxx_right_angle_brackets cxx_thread_local cxx_constexpr)
target_compile_features(seed11 PRIVATE cxx_thread_local)

file(GLOB EXAMPLE_FILES "examples/*.cpp")
file(GLOB TEST_FILES "tests/*.cpp")

foreach(sample ${EXAMPLE_FILES})
	get_filename_component(sample_we ${sample} NAME_WE)
	add_executable(${sample_we} ${sample})
	set_property(TARGET ${sample_we} PROPERTY CXX_STANDARD 11)
	target_link_libraries(${sample_we} seed11)
endforeach(sample)

foreach(sample ${TEST_FILES})
	get_filename_component(sample_we ${sample} NAME_WE)
	add_executable(${sample_we} ${sample})
	find_package(Threads)
	set_property(TARGET ${sample_we} PROPERTY CXX_STANDARD 11)
	target_link_libraries(${sample_we} seed11 Threads::Threads)
endforeach(sample)