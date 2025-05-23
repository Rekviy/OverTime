# engine
set(OVERTIME_VERSION 0.1)

project(OT_libs)

include(src/core/CMakeLists.txt)
include(src/events/CMakeLists.txt)
include(src/imGui/CMakeLists.txt)
include(src/platform/CMakeLists.txt)
include(src/renderer/CMakeLists.txt)
include(src/scripting/CMakeLists.txt)

set(OT_HEADER src/overtime.h src/pch.h
    ${OT_CORE_HEADER}
    ${OT_EVENTS_HEADER}
    ${OT_IMGUI_HEADER}
    ${OT_PLATFORM_HEADER}
    ${OT_RENDERER_HEADER}
    ${OT_SCRIPTING_HEADER}

)
set(OT_SRC src/pch.h
    ${OT_CORE_SRC}
    ${OT_EVENTS_SRC}
    ${OT_IMGUI_SRC}
    ${OT_PLATFORM_SRC}
    ${OT_RENDERER_SRC}
    ${OT_SCRIPTING_SRC}
)
list(APPEND OT_SRC ${OT_HEADER})

add_library(overtime STATIC ${OT_SRC})

add_subdirectory(external ${PROJECT_BINARY_DIR}/external)
target_include_directories(overtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(overtime PRIVATE
    "$<$<CONFIG:Debug>:OT_DEBUG>"
)
if(WINDOWS)
target_compile_definitions(overtime PRIVATE OT_PLATFORM_WINDOWS)
endif()
if(LINUX)
target_compile_definitions(overtime PRIVATE OT_PLATFORM_LINUX)
endif()
set_target_properties(overtime PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    VERSION "${OVERTIME_VERSION}"
                    FOLDER "Internal"
                    )


if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    message("CMake 3.16 target_precompile_headers")
    target_precompile_headers(overtime PRIVATE src/pch.h)
endif()
