cmake_minimum_required(VERSION 3.13)

project(
  Sprout
  VERSION 1.0
  LANGUAGES CXX)


set(ROOT_DIR /cvmfs/hadessoft.gsi.de/install/debian10/root-6.24.02)
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} $ENV{ROOTSYS}/etc/cmake $ENV{ROOTSYS}/cmake/modules/ ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND CMAKE_PREFIX_PATH ${ROOT_DIR} ${ROOT_DIR}/etc/cmake ${ROOT_DIR}/cmake/modules/ ${CMAKE_CURRENT_SOURCE_DIR})
#set(ROOT_DIR /cvmfs/hadessoft.gsi.de/install/debian10/root-6.24.02/etc/cmake)

#Finds ROOT
find_package(ROOT REQUIRED)

#Sets path of source and header files 
set(SRCFILES
    ${PROJECT_SOURCE_DIR}/source/SproutPlot.cxx 
    ${PROJECT_SOURCE_DIR}/source/SproutTree.cxx 
    ${PROJECT_SOURCE_DIR}/source/SproutFit.cxx)

set(HFILES 
    ${PROJECT_SOURCE_DIR}/include/SproutPlot.h 
    ${PROJECT_SOURCE_DIR}/include/SproutTree.h 
    ${PROJECT_SOURCE_DIR}/include/SproutFit.h
    ${PROJECT_SOURCE_DIR}/include/SproutParam.h)
  
include(GNUInstallDirs) #Adds a bunch of install variables to the cmake cache
#set(CMAKE_INSTALL_PREFIX /home/malin/hades/software/src/TestInstall)


#Include this to access the cmake functions from ROOT
include(${ROOT_USE_FILE})

#Dictionary needed to run code with ROOT's Cling interpreter 
ROOT_GENERATE_DICTIONARY(SproutDict ${HFILES} MODULE ${PROJECT_NAME} LINKDEF include/SproutLinkDef.h)

#Adds and links the project library with ROOT 
add_library(Sprout SHARED ${SRCFILES} SproutDict.cxx ${HFILES}) #SproutDict.cxx is created when calling ROOT_GENERATE_DICTIONARY

target_include_directories( #Tells the project where to find include files (avoid hard-coding paths)
    Sprout
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" #sets different include destinations depending on if building or installing 
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
) 
target_link_libraries(Sprout ${ROOT_LIBRARIES})

install(TARGETS 
    Sprout 
    EXPORT SproutTargets #Only prepares the SproutTargets 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(EXPORT #Creates and installs the SproutTarget at the correct location 
    SproutTargets
    FILE SproutTargets.cmake
    NAMESPACE Sprout::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sprout
)

#Install the header files to same destination as that in target_include_directories 
install(FILES ${HFILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}) 

#Root wants this file in the same folder as the lib 
install(FILES build/libSprout_rdict.pcm DESTINATION ${CMAKE_INSTALL_LIBDIR})

#To use any library, anywhere, all that's needed is a library file and the header files. 
#The nessecary targets can then be IMPORTED for use in another project. You can IMPORT
#the library target itself but this requires a hard-coded path to the installed lib file. This
#becomes tricky when files of this project are installed or moved around 
#To take care of this, cmake can generate (EXPORT) and install a *Targets.cmake 
#file which helps the new project import all the relevant targets regardless of where it is located. 



