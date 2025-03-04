# engine
cmake_minimum_required(VERSION 3.13)


set(OVERTIME_VERSION 0.1)


project(OT_libs)

include(src/core/CMakeLists.txt)
include(src/events/CMakeLists.txt)
include(src/imGui/CMakeLists.txt)
include(src/platform/${PLATFORM_FOLDER}/CMakeLists.txt)
set(OT_HEADER src/overtime.h src/pch.h
	${OT_CORE_HEADER}
	${OT_EVENTS_HEADER}
    ${OT_IMGUI_HEADER}
    ${OT_PLATFORM_HEADER}
)
set(OT_SRC src/pch.h
	${OT_CORE_SRC}
	${OT_EVENTS_SRC}
    ${OT_IMGUI_SRC}
    ${OT_PLATFORM_SRC}
)
list(APPEND OT_SRC ${OT_HEADER})

add_library(overtime SHARED ${OT_SRC})

add_subdirectory(external ${PROJECT_BINARY_DIR}/external)
target_include_directories(overtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
#target_link_libraries(overtime external)
target_compile_definitions(overtime PRIVATE
	"OT_PLATFORM_WINDOWS"
	"OT_BUILD_DLL"
    "OT_DEBUG"
)
set_target_properties(overtime
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      VERSION "${OVERTIME_VERSION}"
                      FOLDER "Internal"
                      )


if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    message("CMake 3.16 target_precompile_headers")
    target_precompile_headers(overtime PRIVATE src/pch.h)
endif()
