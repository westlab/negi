cmake_minimum_required(VERSION 2.8.4)
project(negi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
file(GLOB negiSrcs "*.cpp")

add_executable(negi ${negiSrcs})
target_link_libraries(negi pcap)
target_link_libraries(negi z)
target_link_libraries(negi sqlite3)


