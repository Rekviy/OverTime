project(stb_image)
add_library(${PROJECT_NAME} STATIC stb_image.cpp stb_image.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})