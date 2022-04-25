cmake_minimum_required(VERSION 3.2)
project(c-app C)

# C standard
set(CMAKE_C_STANDARD 99)

# Runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../product)

# Set debug
set(CMAKE_BUILD_TYPE Debug)

# Set debug macro
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

# Src directory
include_directories(src)

# Source files
set(SOURCE_FILES

    # src
    src/main.c src/common.h

    # shrain-std
    src/shrain-std/util.h
    src/shrain-std/list.h
    src/shrain-std/string.h
    src/shrain-std/map.h            src/shrain-std/map.c

    # shrain-std/parser
    src/shrain-std/parser/parser.c  src/shrain-std/parser/parser.h
)

# Executable
add_executable(c-app ${SOURCE_FILES})

# Link libraries
#target_link_libraries()