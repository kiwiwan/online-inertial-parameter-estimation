# Copyright: 2010-2011 RobotCub Consortium
# Author: Serena Ivaldi
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
# 

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECTNAME oneChainInvSensorRegressor)

PROJECT(${PROJECTNAME})

FIND_PACKAGE(YARP)
FIND_PACKAGE(ICUB)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${YARP_MODULE_PATH})
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ICUB_MODULE_PATH})
INCLUDE(iCubOptions)
INCLUDE(iCubHelpers)

SET(folder_source main.cpp)

SOURCE_GROUP("Source Files" FILES ${folder_source})

INCLUDE_DIRECTORIES(${ICUB_INCLUDE_DIRS}
                    ${YARP_INCLUDE_DIRS})
					
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ICUB_LINK_FLAGS}")					

ADD_EXECUTABLE(${PROJECTNAME} ${folder_source})

TARGET_LINK_LIBRARIES(${PROJECTNAME} iDyn
                                     ${YARP_LIBRARIES})

