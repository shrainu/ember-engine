cmake_minimum_required(VERSION 3.2)
project(ember-engine C)

# C standard
set(CMAKE_C_STANDARD 99)

# Runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../product)

# Library output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../product/lib)

# Set debug
#set(CMAKE_BUILD_TYPE Debug)

# Set debug macro
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

# GLFW
include_directories(/opt/homebrew/Cellar/glfw/3.3.6/include)
link_directories(/opt/homebrew/Cellar/glfw/3.3.6/lib)

# GLEW
include_directories(/opt/homebrew/Cellar/glew/2.2.0_1/include)
link_directories(/opt/homebrew/Cellar/glew/2.2.0_1/lib)

# Freetype
include_directories(/opt/homebrew/Cellar/freetype/2.11.1/include/freetype2)
link_directories(/opt/homebrew/Cellar/freetype/2.11.1/lib)

# Common
include_directories(src/)

# Vendor
include_directories(src/vendor)

# STB Image files
set(STB_IMAGE_FILES

    src/vendor/stb_image/stb_image.c
)

# Source files
set(SOURCE_FILES

    # ember
    src/ember.h

    # src
    src/main.c src/common.h

    # ember-std
    src/ember-std/map.h
    src/ember-std/list.h
    src/ember-std/util.h
    src/ember-std/string.h

    # ember-std/parser
    src/ember-std/parser/parser.c   src/ember-std/parser/parser.h

    # engine
    src/engine/font.c src/engine/font.h
    src/engine/engine.c src/engine/engine.h
    src/engine/window.c src/engine/window.h
    src/engine/camera.c src/engine/camera.h
    src/engine/shader.c src/engine/shader.h
    src/engine/texture.c src/engine/texture.h
    src/engine/renderer.c src/engine/renderer.h
)

# Executable
add_executable(ember-engine ${SOURCE_FILES} ${STB_IMAGE_FILES})

# Library
#add_library(ember-engine ${SOURCE_FILES} ${STB_IMAGE_FILES})

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(ember-engine
    OpenGL::GL
    freetype.a
    GLEW.a
    glfw
)