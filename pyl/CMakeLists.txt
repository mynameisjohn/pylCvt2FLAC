CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Project needs c++14 for std::integer_sequence
SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

# Specify the python version
# Be wary of doing something like
# 3.5 instead of 35, etc.
MESSAGE("\n")
IF(NOT(${_PY_VER}))
    SET(_PY_VER "35")
    ADD_DEFINITIONS("-D_PY_VER=${_PY_VER}")
    MESSAGE("PyLiaison: No Python version specified! Defaulting to ${_PY_VER}")
ELSE(NOT(${_PY_VER}))
    MESSAGE("PyLiaison: Python version specified as ${_PY_VER}")
ENDIF(NOT(${_PY_VER}))

# Set define flag for debug builds
# This is important for windows, as combining
# release and debug code with python crashes
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    ADD_DEFINITIONS(-D_PYL_DBG)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Set library directory
# uses version specified and some familiar paths,
# but can be specified on the command line
IF(NOT(${_PYTHON_LIB_DIR}))
    IF (WIN32)
        SET(_PYTHON_LIB_DIR "C:/Python${_PY_VER}/libs")
    ELSEIF(APPLE)
        SET(_PYTHON_LIB_DIR "/usr/local/lib")
    ELSEIF(UNIX)
        SET(_PYTHON_LIB_DIR "/usr/lib/x86_64-linux-gnu")
    ENDIF(WIN32)
    MESSAGE("PyLiaison: No Python library dir specified! Defaulting to ${_PYTHON_LIB_DIR}")
ELSE(NOT(${_PYTHON_LIB_DIR}))
    MESSAGE("PyLiaison: Python library dir set to ${_PYTHON_LIB_DIR}")
ENDIF(NOT(${_PYTHON_LIB_DIR}))

# Set actual library file
IF(WIN32)
    # We must link against the correct
    # library on Windows, or crash
    IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        SET(PYTHON_LIB "python${_PY_VER}_d")
    ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        SET(PYTHON_LIB "python${_PY_VER}")
    ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
ELSE(WIN32)
    # Otherwise link against the shared object
    SET(PYTHON_LIB "python${_PY_VER}m")
ENDIF(WIN32)

MESSAGE("PyLiaison: Python library set to ${PYTHON_LIB}")

# Set include directory
# uses version specified and some familiar paths,
# but can be specified on the command line
IF(NOT(${_PYTHON_INCLUDE_DIR}))
    IF (WIN32)
        SET(_PYTHON_INCLUDE_DIR "C:/Python${_PY_VER}/include")
    ELSEIF(APPLE)
        SET(_PYTHON_INCLUDE_DIR "/usr/local/include/python${_PY_VER}m")
    ELSEIF(UNIX)
        SET(_PYTHON_INCLUDE_DIR "/usr/include/python${_PY_VER}m")
    ENDIF(WIN32)
    MESSAGE("PyLiaison: No Python include dir specified! Defaulting to ${_PYTHON_INCLUDE_DIR}")
ELSE(NOT(${_PYTHON_INCLUDE_DIR}))
    MESSAGE("PyLiaison: Python include dir set to ${_PYTHON_INCLUDE_DIR}")
ENDIF(NOT(${_PYTHON_INCLUDE_DIR}))
MESSAGE("\n")

# Declare PyLiaison library
ADD_LIBRARY(PyLiaison
    ${CMAKE_CURRENT_SOURCE_DIR}/pyliaison.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pyliaison.h)

# We need the include path and the python header file location
TARGET_INCLUDE_DIRECTORIES(PyLiaison PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${_PYTHON_INCLUDE_DIR})

# Create variables for parent scope
# to allow easier include/linkage
SET(PYTHON_INCLUDE_DIR ${_PYTHON_INCLUDE_DIR} PARENT_SCOPE)
SET(PYTHON_LIB_DIR ${_PYTHON_LIB_DIR} PARENT_SCOPE)
SET(PYTHON_LIB ${PYTHON_LIB} PARENT_SCOPE)
