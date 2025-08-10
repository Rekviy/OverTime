# application
set(APP_NAME SandBox)

project(${APP_NAME})

set(TEST_SOURCE src/testApp.cpp
    src/test2d.cpp
)
set(TEST_HEADER src/testApp.h
    src/test2d.h
)

set(TEST_RES
    "assets/resources/cherry.png"
    "assets/resources/Screenshot.png"    
)
set(TEST_SHADERS
    "assets/shaders/texture.vert"
    "assets/shaders/texture.frag"
)

list(APPEND TEST_RES ${TEST_SHADERS})

list(APPEND TEST_SOURCE ${TEST_HEADER} ${TEST_RES})

add_executable(${APP_NAME} ${TEST_SOURCE})

foreach(item IN LISTS TEST_RES)
         res_mgmt(${APP_NAME} ${item})
endforeach()

set_target_properties(${APP_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    FOLDER "Internal"
                    RESOURCE "${TEST_RES}" 
                    )

#include_directories(${OVERTIME_ROOT_PATH}/OverTime/src)
target_link_libraries(${APP_NAME} PRIVATE overtime)
if(WINDOWS)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_WINDOWS)
endif()
if(LINUX)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_LINUX)
endif()
