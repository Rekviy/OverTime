# application
set(APP_NAME Breeze)

project(${APP_NAME})

set(GAME_SOURCE src/Breeze.cpp
src/gameLayer.cpp
src/ui/gameUI.cpp
src/cameraWrapper.cpp
src/ui/button.cpp
src/ui/themeManager.cpp
src/ui/interactElement.cpp
src/ui/counter.cpp
src/ui/objectPool.cpp
src/ui/grid.cpp
src/ui/maskLayer.cpp
src/ui/ship.cpp
src/gridManager.cpp
src/gameStateMachine.cpp
)
set(GAME_HEADER 
src/gameLayer.h
src/ui/gameUI.h
src/cameraWrapper.h
src/ui/button.h
src/ui/themeManager.h
src/ui/interactElement.h
src/ui/counter.h
src/ui/objectPool.h
#src/ui/IObjectPool.h
src/ui/grid.h
src/ui/maskLayer.h
src/ui/ship.h
src/gridManager.h
src/stateMachine.h
src/gameStateMachine.h
)

list(APPEND GAME_SOURCE ${GAME_HEADER} ${GAME_RES})

add_executable(${APP_NAME} ${GAME_SOURCE})

file(GLOB_RECURSE GAME_RES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS 
"assets/**/*.png" "assets/**/*.frag" "assets/**/*.vert" 
)

foreach(item IN LISTS GAME_RES)
         res_mgmt(${APP_NAME} ${item})
endforeach()


target_compile_definitions(${APP_NAME} PRIVATE
    "$<$<CONFIG:Debug>:OT_DEBUG>"
)

target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(${APP_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    FOLDER "Internal"
                    RESOURCE "${GAME_RES}" 
                    CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                    )

target_link_libraries(${APP_NAME} PRIVATE overtime)
if(WINDOWS)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_WINDOWS)
endif()
if(LINUX)
target_compile_definitions(${APP_NAME} PRIVATE OT_PLATFORM_LINUX)
endif()
