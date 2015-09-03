project(GeometricTools)
cmake_minimum_required(VERSION 2.8)

# Static by default.
set (BUILD_SHARED_LIBS OFF)
add_definitions(-DUNICODE)

include_directories (GTEngine/Include)
add_subdirectory (GTEngine/Source)

install (DIRECTORY GTEngine/Include DESTINATION include FILES_MATCHING PATTERN "*.h")