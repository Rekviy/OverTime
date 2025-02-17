cmake_minimum_required(VERSION 3.13)

project(external)

option(BUILD_LOGGING "Build with spdlog logging" ON)

if(BUILD_LOGGING)
	add_subdirectory(spdlog)
	target_link_libraries(overtime spdlog)
endif(BUILD_LOGGING)