cmake_minimum_required(VERSION 3.0)

project(RHelper)

#Finds ROOT
find_package(ROOT REQUIRED)
if ( NOT ROOT_FOUND )
message(FATAL_ERROR "ROOT is required but missing !")
endif ( NOT ROOT_FOUND )

#Sets path of source and header files 
set(SRCFILES ${PROJECT_SOURCE_DIR}/source/HistogramHelper.cxx ${PROJECT_SOURCE_DIR}/source/RHtree.cxx ${PROJECT_SOURCE_DIR}/source/RHfit.cxx)
set(HFILES ${PROJECT_SOURCE_DIR}/include/HistogramHelper.h ${PROJECT_SOURCE_DIR}/include/RHtree.h ${PROJECT_SOURCE_DIR}/include/RHfit.h)

#Include this to access the cmake functions from ROOT
include(${ROOT_USE_FILE})

#Dictionary needed to run code with ROOT's Cling interpreter 
ROOT_GENERATE_DICTIONARY(RHelperDict ${HFILES} LINKDEF include/RHelperLinkDef.h)

#Adds and links the project library with ROOT 
add_library(RHelper SHARED ${SRCFILES} RHelperDict.cxx) # RHelperDict.cxx is created when calling ROOT_GENERATE_DICTIONARY
target_include_directories(RHelper PRIVATE ${CMAKE_SOURCE_DIR}/include) #Tells the project where to find include files (avoid hard-coding paths)
target_link_libraries(RHelper ${ROOT_LIBRARIES})





