cmake_minimum_required(VERSION 3.13)
project(imgui)
set(IMGUI_HEADER imgui/imconfig.h imgui/imgui.h imgui/imgui_internal.h imgui/imstb_rectpack.h imgui/imstb_textedit.h imgui/imstb_truetype.h)
set(IMGUI_SRC imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_demo.cpp)
set(IMGUI_OPENGL_BACK
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_glfw.cpp
)
list(APPEND IMGUI_SRC ${IMGUI_HEADER} ${IMGUI_OPENGL_BACK})
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
