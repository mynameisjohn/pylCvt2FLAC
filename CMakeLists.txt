CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(pylCvt2FLAC)

SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

file(GLOB SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/*.py)
source_group("Scripts" FILES ${SCRIPTS})

# Add subdir for Pyliaison
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pyl)

# Set up include / lib directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR})
LINK_DIRECTORIES(${PYTHON_LIB_DIR})

# Create project executable
ADD_EXECUTABLE(pylCvt2FLAC ${CMAKE_CURRENT_SOURCE_DIR}/pylCvt2FLAC.cpp ${SCRIPTS})

# Add libsndfile for Windows
IF(WIN32)
	SET(_WIN_XIPH_LIBS 
		"$ENV{SDK}/libsndfile/lib/$(Platform)/$(Configuration)/libsndfile.lib"
		"$ENV{SDK}/XIPH/vorbis/lib/$(Platform)/$(Configuration)/libvorbis_static.lib"
		"$ENV{SDK}/XIPH/FLAC/lib/$(Platform)/$(Configuration)/libFLAC_static.lib"
		"$ENV{SDK}/XIPH/ogg/lib/$(Platform)/$(Configuration)/libogg_static.lib")
	TARGET_LINK_LIBRARIES(pylCvt2FLAC LINK_PUBLIC PyLiaison ${PYTHON_LIB} ${_WIN_XIPH_LIBS})
	TARGET_INCLUDE_DIRECTORIES(pylCvt2FLAC PUBLIC "$ENV{SDK}/libsndfile/include")
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(pylCvt2FLAC LINK_PUBLIC PyLiaison ${PYTHON_LIB} sndfile)
ENDIF(WIN32)

