CMAKE_MINIMUM_REQUIRED(VERSION 3.20 FATAL_ERROR)

PROJECT(modelviewer)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)

SET(_target ${PROJECT_NAME})

FILE(GLOB_RECURSE
    _source_list
    "source/*.h"
    "source/*.c"
)

ADD_EXECUTABLE(
    ${_target}
    ${_source_list}
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PRIVATE
        OpenGL::GL
        SDL2::SDL2
        SDL2::SDL2main
)

TARGET_INCLUDE_DIRECTORIES(
    ${_target}
    PRIVATE
        source
        glmm/include
)