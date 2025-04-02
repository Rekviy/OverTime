cmake_minimum_required(VERSION 3.13)
project(imgui)
set(IMGUI_HEADER imconfig.h imgui.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)
set(IMGUI_SRC imgui.cpp imgui_draw.cpp imgui_widgets.cpp imgui_tables.cpp imgui_demo.cpp)
set(IMGUI_OPENGL_BACK
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_glfw.h
    backends/imgui_impl_glfw.cpp
)
list(APPEND IMGUI_SRC ${IMGUI_HEADER} ${IMGUI_OPENGL_BACK})
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
