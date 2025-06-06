cmake_minimum_required(VERSION 3.31)
project(raylibTesting)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_SOURCES
        externalLibs/imgui-1.91.9b/imgui.cpp
        externalLibs/imgui-1.91.9b/imgui_draw.cpp
        externalLibs/imgui-1.91.9b/imgui_tables.cpp
        externalLibs/imgui-1.91.9b/imgui_widgets.cpp
)

include_directories(raylibTesting
        externalLibs/raylib/raylib-5.5_linux_amd64/include
        externalLibs/raylib/rlImGui
        externalLibs/imgui-1.91.9b
)

add_executable(raylibTesting
        main.cpp
        externalLibs/raylib/rlImGui/rlImGui.cpp
        ${IMGUI_SOURCES}
)

target_link_libraries(raylibTesting
        ${CMAKE_SOURCE_DIR}/externalLibs/raylib/raylib-5.5_linux_amd64/lib/libraylib.a   # raylib
        m        # math library (libm)
        pthread  # threading
        dl       # dynamic loading
        rt       # real-time extensions
        X11      # X Window System
)