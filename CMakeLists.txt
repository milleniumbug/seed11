cmake_minimum_required(VERSION 3.3)
project(seed11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(CXX_EXTENSIONS FALSE)
set(SOURCE_FILES
	src/seed11_system_agnostic.cpp)

if(${WIN32})
	set(SOURCE_FILES ${SOURCE_FILES} src/seed11_windows.cpp)
else(${WIN32})
	set(SOURCE_FILES ${SOURCE_FILES} src/seed11_linux.cpp)
endif(${WIN32})

include_directories(include)

add_library(seed11 ${SOURCE_FILES})
target_compile_features(seed11 INTERFACE cxx_deleted_functions cxx_rvalue_references cxx_defaulted_functions cxx_right_angle_brackets cxx_thread_local)
target_compile_features(seed11 PRIVATE cxx_deleted_functions cxx_rvalue_references cxx_defaulted_functions cxx_right_angle_brackets cxx_thread_local)