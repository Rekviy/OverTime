# project
cmake_minimum_required(VERSION 3.13)


project(OverTime)

set(OVERTIME_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${OVERTIME_ROOT_PATH}/cmake/Modules/)
include(OverTimeConfigDefine)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set("DEBUG=$<CONFIG:Debug>")
if(DEBUG)
add_compile_definitions("OT_ENABLE_ASSERTS"
)
endif(DEBUG)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
add_subdirectory(OverTime)
add_subdirectory(Breeze)