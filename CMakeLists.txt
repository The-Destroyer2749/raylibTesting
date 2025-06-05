cmake_minimum_required(VERSION 3.31)
project(raylibTesting)

set(CMAKE_CXX_STANDARD 20)

include_directories(raylibTesting raylib/raylib-5.5_linux_amd64/include)

add_executable(raylibTesting main.cpp)

target_link_libraries(raylibTesting
        ${CMAKE_SOURCE_DIR}/raylib/raylib-5.5_linux_amd64/lib/libraylib.a   # raylib
        m        # math library (libm)
        pthread  # threading
        dl       # dynamic loading
        rt       # real-time extensions
        X11      # X Window System
)