cmake_minimum_required(VERSION 2.8.12)
project(imgui)

include_directories(imgui "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include")

# Include header files.
include_directories(imgui
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends")

# Include source files.
add_library(imgui
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

