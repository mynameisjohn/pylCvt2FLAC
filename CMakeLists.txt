CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(pylCvt2FLAC)

SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

# Add subdir for Pyliaison
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pyl)

# Set up include / lib directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR})
LINK_DIRECTORIES(${PYTHON_LIB_DIR})

# Add libsndfile for Windows (TODO)

# Create project executable
ADD_EXECUTABLE(pylCvt2FLAC ${CMAKE_CURRENT_SOURCE_DIR}/pylCvt2FLAC.cpp)
TARGET_LINK_LIBRARIES(pylCvt2FLAC LINK_PUBLIC PyLiaison ${PYTHON_LIB} sndfile)
