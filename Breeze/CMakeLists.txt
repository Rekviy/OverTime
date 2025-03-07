# application
set(APP_NAME Breeze)

project(${APP_NAME})

set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

list(APPEND GAME_SOURCE src/Breeze.cpp)
list(APPEND GAME_HEADER src/Breeze.h)

list(APPEND GAME_SOURCE ${GAME_HEADER})


add_executable(${APP_NAME} ${GAME_SOURCE})

set_target_properties(${APP_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    FOLDER "Internal"
                    )

#include_directories(${OVERTIME_ROOT_PATH}/OverTime/src)
target_link_libraries(${APP_NAME} PRIVATE overtime)
if(WINDOWS)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_WINDOWS)
endif()
if(LINUX)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_LINUX)
endif()
