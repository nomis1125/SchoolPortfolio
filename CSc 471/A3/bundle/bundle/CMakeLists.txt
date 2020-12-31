cmake_minimum_required(VERSION 3.12)
project(a2 LANGUAGES CXX C)

set(ASSIGNMENT_ROOT "${PROJECT_SOURCE_DIR}")

include(FetchContent)
FetchContent_Declare(
    atlas
    GIT_REPOSITORY https://github.com/marovira/atlas.git)
FetchContent_Populate(atlas)
add_subdirectory(${atlas_SOURCE_DIR} ${atlas_BINARY_DIR})

set(ASSIGNMENT_INCLUDE
    "${ASSIGNMENT_ROOT}/assignment.hpp"
    "${ASSIGNMENT_ROOT}/UniformLocations.glsl"
    )
set(ASSIGNMENT_SOURCE 
    "${ASSIGNMENT_ROOT}/main.cpp"
    )

set(PATH_INCLUDE "${ASSIGNMENT_ROOT}/Paths.hpp")
configure_file("${ASSIGNMENT_ROOT}/Paths.hpp.in" ${PATH_INCLUDE})
list(APPEND ASSIGNMENT_INCLUDE ${PATH_INCLUDE})

source_group("include" FILES ${ASSIGNMENT_INCLUDE})
source_group("source" FILES ${ASSIGNMENT_SOURCE})

add_executable(a2 ${ASSIGNMENT_INCLUDE} ${ASSIGNMENT_SOURCE} ${ASSIGNMENT_SHADER})
target_link_libraries(a2 PUBLIC atlas::atlas)

