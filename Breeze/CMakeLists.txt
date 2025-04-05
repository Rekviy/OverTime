# application
set(APP_NAME Breeze)

project(${APP_NAME})

set(GAME_SOURCE src/Breeze.cpp

)
set(GAME_HEADER src/Breeze.h

)

set(GAME_RES
    "assets/resources/cherry.png"
    "assets/resources/Screenshot.png"    
)
set(GAME_SHADERS
    "assets/shaders/texture.vert"
    "assets/shaders/texture.frag"
)

list(APPEND GAME_RES ${GAME_SHADERS})

list(APPEND GAME_SOURCE ${GAME_HEADER} ${GAME_RES})

add_executable(${APP_NAME} ${GAME_SOURCE})

foreach(item IN LISTS GAME_RES)
         res_mgmt(${APP_NAME} ${item})
endforeach()

set_target_properties(${APP_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    FOLDER "Internal"
                    RESOURCE "${GAME_RES}" 
                    )

#include_directories(${OVERTIME_ROOT_PATH}/OverTime/src)
target_link_libraries(${APP_NAME} PRIVATE overtime)
if(WINDOWS)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_WINDOWS)
endif()
if(LINUX)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_LINUX)
endif()
