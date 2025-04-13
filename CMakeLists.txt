# project
cmake_minimum_required(VERSION 3.13)

project(OverTime)

set(OVERTIME_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${OVERTIME_ROOT_PATH}/cmake/Modules/)
include(OverTimeConfigDefine)


add_subdirectory(OverTime)

option(BUILD_GAME "build game or clientSideTest" ON)

if(BUILD_GAME)
	add_subdirectory(Breeze)
else()
	add_subdirectory(clientSideTest)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
