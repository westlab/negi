cmake_minimum_required(VERSION 2.8.4)
project(negi)


# try enable c++11
# g++ and clang++ need flags to enable c++11
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" HAS_CPP11)
if (HAS_CPP11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


file(GLOB negiSrcs "./src/*.cpp")

# add includes for all
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

# add db
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/db")
# include_directories(db)

# db
#set(DB "${CMAKE_CURRENT_SOURCE_DIR}/db/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/db/*.h")


# add gflag
add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/external/gflags")
# add includes for gflags
include_directories ("${gflags_BINARY_DIR}/include")
# add library path for gflags
set(CMAKE_LIBRARY_PATH "${gflags_BINARY_DIR}/lib" ${CMAKE_LIBRARY_PATH})

# add google-glog
add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/external/google-glog")
# add includes for google-glog
include_directories ("${google-glog_SOURCE_DIR}/src")
# add library path for google-glog
set(CMAKE_LIBRARY_PATH "${google-glog_BINARY_DIR}" ${CMAKE_LIBRARY_PATH})

# add gtest
add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/external/gtest")
# add includes for gtests
include_directories ("${gtest_SOURCE_DIR}/src")
# add library path for gtest
set(CMAKE_LIBRARY_PATH "${gtest_BINARY_DIR}" ${CMAKE_LIBRARY_PATH})


add_executable(negi ${negiSrcs} ${SOURCE} ${HEADERS})
target_link_libraries(negi pcap)
target_link_libraries(negi z)
target_link_libraries(negi sqlite3)
target_link_libraries(negi glog)

if (test)
    add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/test")
    enable_testing()
    file(GLOB TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
    add_executable(runUnitTests ${TEST_SRC_FILES} ${negiSrcs} ${SOURCE} ${HEADERS})
    target_link_libraries(runUnitTests pthlead)
    target_link_libraries(runUnitTests gtest)
    add_test(NAME that-test-I-made COMMAND runUnitTests)
endif()
