# application
set(APP_NAME Breeze)

project(${APP_NAME})

set(GAME_SOURCE src/Breeze.cpp
src/gameLayer.cpp
src/player.cpp
src/uiLayer.cpp
src/cameraWrapper.cpp
src/ui/button.cpp
src/ui/themeManager.cpp
)
set(GAME_HEADER 
src/gameLayer.h
src/player.h
src/uiLayer.h
src/cameraWrapper.h
src/ui/button.h
src/ui/themeManager.h
src/ui/interactElement.h
)

set(GAME_RES
    "assets/resources/cherry.png"
    "assets/resources/Screenshot.png"    
    "assets/resources/Proxy555.jpg"    
    "assets/resources/frame.png"    
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

target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(${APP_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    FOLDER "Internal"
                    RESOURCE "${GAME_RES}" 
                    )

target_link_libraries(${APP_NAME} PRIVATE overtime)
if(WINDOWS)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_WINDOWS)
endif()
if(LINUX)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_LINUX)
endif()
