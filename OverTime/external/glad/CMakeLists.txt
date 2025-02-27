cmake_minimum_required(VERSION 3.13)
project(glad)

add_library(glad STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(glad
                      PROPERTIES
                      FOLDER "External"
                      )