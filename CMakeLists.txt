cmake_minimum_required(VERSION 3.10)  # 3.30 is too high, use 3.10+
project(CSS4422-Driver-Project-Team-8 C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 11)

# Define the correct executable target
add_executable(CSS4422-Driver-Project-Team-8 ArmUI.c)

# Link GTK to the correct target
target_link_libraries(CSS4422-Driver-Project-Team-8 ${GTK3_LIBRARIES})
