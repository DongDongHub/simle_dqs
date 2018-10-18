cmake_minimum_required(VERSION 2.8) 
set(SRC_LIST  cli.cpp)
add_executable(cli ${SRC_LIST}) 
target_link_libraries(cli ppconsul curl thrift_dqs thrift boost_regex json11)       

#set(SRC_LIST dqs_constants.cpp  dqs_constants.h  dqsService.cpp  dqsService.h  dqs_types.cpp  dqs_types.h  cli.cpp)
#add_executable(cli ${SRC_LIST}) 
#target_link_libraries(cli thrift boost_thread-mt boost_filesystem boost_system-mt)       
