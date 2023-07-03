cmake_minimum_required(VERSION 3.0)

project(Sprout)

#Finds ROOT
find_package(ROOT REQUIRED)
if ( NOT ROOT_FOUND )
message(FATAL_ERROR "ROOT is required but missing !")
endif ( NOT ROOT_FOUND )

#Sets path of source and header files 
set(SRCFILES
    ${PROJECT_SOURCE_DIR}/source/SproutPlot.cxx 
    ${PROJECT_SOURCE_DIR}/source/SproutTree.cxx 
    ${PROJECT_SOURCE_DIR}/source/SproutFit.cxx)

set(HFILES 
    ${PROJECT_SOURCE_DIR}/include/SproutPlot.h 
    ${PROJECT_SOURCE_DIR}/include/SproutTree.h 
    ${PROJECT_SOURCE_DIR}/include/SproutFit.h)

#Include this to access the cmake functions from ROOT
include(${ROOT_USE_FILE})

#Dictionary needed to run code with ROOT's Cling interpreter 
ROOT_GENERATE_DICTIONARY(SproutDict ${HFILES} LINKDEF include/SproutLinkDef.h)

#Adds and links the project library with ROOT 
add_library(Sprout SHARED ${SRCFILES} SproutDict.cxx) # RHelperDict.cxx is created when calling ROOT_GENERATE_DICTIONARY
target_include_directories(Sprout PRIVATE ${CMAKE_SOURCE_DIR}/include) #Tells the project where to find include files (avoid hard-coding paths)
target_link_libraries(Sprout ${ROOT_LIBRARIES})





