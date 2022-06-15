cmake_minimum_required(VERSION 2.8)
project(ORB_SLAM2)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(OpenCV 4 REQUIRED HINTS ${CMAKE_SOURCE_DIR}/Thirdparty/opencv/install/Debug/lib/cmake/opencv4)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()


find_package(Pangolin REQUIRED HINTS ${CMAKE_SOURCE_DIR}/Thirdparty/pangolin/install/Debug/lib/cmake/Pangolin)
if (Pangolin_FOUND)
   MESSAGE(STATUS "Pangolin FOUND! - ${Pangolin_DIR}")
endif()
find_package(Eigen3 REQUIRED HINTS ${CMAKE_SOURCE_DIR}/Thirdparty/eigen/install/Debug/share/eigen3/cmake)
if (Eigen3_FOUND)
   MESSAGE(STATUS "Eigen3 FOUND! - ${Eigen3_DIR}")
   set(Eigen3_LIBS Eigen3::Eigen)
endif()
find_package(easy_profiler REQUIRED HINTS ${CMAKE_SOURCE_DIR}/Thirdparty/easy_profiler/install/lib/cmake/easy_profiler)
if(${easy_profiler_FOUND})
   message(STATUS "Found: Easy-profiler - ${easy_profiler_DIR}")
endif(${easy_profiler_FOUND})

set(EASY_PROFILER_LIBS easy_profiler)
set(BUILD_WITH_EASY_PROFILER true)


include_directories(
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
${EIGEN3_INCLUDE_DIR}
${Pangolin_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/Thirdparty/easy_profiler/install/include
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} SHARED
src/System.cc
src/Tracking.cc
src/LocalMapping.cc
src/LoopClosing.cc
src/ORBextractor.cc
src/ORBmatcher.cc
src/FrameDrawer.cc
src/Converter.cc
src/MapPoint.cc
src/KeyFrame.cc
src/Map.cc
src/MapDrawer.cc
src/Optimizer.cc
src/PnPsolver.cc
src/Frame.cc
src/KeyFrameDatabase.cc
src/Sim3Solver.cc
src/Initializer.cc
src/Viewer.cc
)

target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
${EIGEN3_LIBS}
${Pangolin_LIBRARIES}
easy_profiler
${PROJECT_SOURCE_DIR}/Thirdparty/DBoW2/lib/libDBoW2.so
${PROJECT_SOURCE_DIR}/Thirdparty/g2o/lib/libg2o.so
)

# Build examples



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples)

add_executable(mono_kitti
Examples/mono_kitti.cc)
target_link_libraries(mono_kitti ${PROJECT_NAME})