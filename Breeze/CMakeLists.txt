# application
cmake_minimum_required(VERSION 3.13)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(APP_NAME Breeze)

project(${APP_NAME})

set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

list(APPEND GAME_SOURCE src/Breeze.cpp)
list(APPEND GAME_HEADER src/Breeze.h)
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )


add_executable(${APP_NAME} ${all_code_files})

set_target_properties(${APP_NAME}
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "Internal"
                      )

include_directories(${OVERTIME_ROOT_PATH}/OverTime/src)
target_link_libraries(${APP_NAME} PRIVATE overtime)
target_compile_definitions(${APP_NAME} PRIVATE
    OT_PLATFORM_WINDOWS
)
