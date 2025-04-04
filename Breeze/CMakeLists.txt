# application
set(APP_NAME Breeze)

project(${APP_NAME})

set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES
    "assets/resources/cherry.png"
    "assets/resources/Screenshot.png"    
)
set(GAME_SHADERS
    "assets/shaders/texture.vert"
    "assets/shaders/texture.frag"
)

foreach(item IN LISTS GAME_RES)
        FILE(COPY ${item} DESTINATION "${CMAKE_BINARY_DIR}/bin/assets/resources")
endforeach()
foreach(item IN LISTS GAME_SHADERS)
        FILE(COPY ${item} DESTINATION "${CMAKE_BINARY_DIR}/bin/assets/shaders")
endforeach()

list(APPEND GAME_SOURCE ${GAME_HEADER})

list(APPEND GAME_SOURCE src/Breeze.cpp)
list(APPEND GAME_HEADER src/Breeze.h)

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
