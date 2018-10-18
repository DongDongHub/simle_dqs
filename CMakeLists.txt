cmake_minimum_required(VERSION 2.8) 
project(dqs) 


set(JIOT_COMMON_LIB_DIR /home/vagrant/jiot/jiot-cpp-common)
set(JIOT_THRIFT_DIR ${JIOT_COMMON_LIB_DIR}/thirdparty/thrift) 

add_definitions(-Wno-builtin-macro-redefined -std=c++11) 
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror  -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_BUILD_TYPE "Debug") 


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})

#add_subdirectory( third_libs/add_subdir  )

include_directories( 
include 
${JIOT_THRIFT_DIR}/include 
${JIOT_THRIFT_DIR}/include/thrift
${CMAKE_CURRENT_SOURCE_DIR}/third_libs/consul/include
${CMAKE_CURRENT_SOURCE_DIR}/third_libs/thrift
)

link_directories( 
${CMAKE_CURRENT_SOURCE_DIR}/lib 
${JIOT_THRIFT_DIR}/lib
${JIOT_COMMON_LIB_DIR}/server/external-el7/lib
${CMAKE_CURRENT_SOURCE_DIR}/lib
${CMAKE_CURRENT_SOURCE_DIR}/third_libs/consul/lib
)

#gen library
add_subdirectory(third_libs)

#gen bin
add_subdirectory(svr)
add_subdirectory(cli) 
