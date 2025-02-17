# engine
cmake_minimum_required(VERSION 3.13)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OVERTIME_VERSION 0.1)



project(OT_libs)
include(src/core/CMakeLists.txt)

set(OT_HEADER src/overtime.h
	${OT_CORE_HEADER}
)
set(OT_SRC src/overtime.cpp
	${OT_CORE_SRC}
)
list(APPEND OT_SRC ${OT_HEADER})

add_library(overtime SHARED ${OT_SRC})

add_subdirectory(external ${PROJECT_BINARY_DIR}/external)
target_include_directories(overtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
#target_link_libraries(overtime external)
target_compile_definitions(overtime PRIVATE
	OT_PLATFORM_WINDOWS
	OT_BUILD_DLL
)
