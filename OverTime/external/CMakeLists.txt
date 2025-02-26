cmake_minimum_required(VERSION 3.13)

project(external)

option(BUILD_LOGGING "Build with spdlog logging" ON)

if(BUILD_LOGGING)
	add_subdirectory(spdlog)
	target_link_libraries(overtime spdlog)
	set_target_properties(spdlog PROPERTIES FOLDER "External")
endif(BUILD_LOGGING)



if(WINDOWS OR MACOSX OR LINUX)
    add_subdirectory(glfw)
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(overtime glfw OpenGL::GL)
endif()