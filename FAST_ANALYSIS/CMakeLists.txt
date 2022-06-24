cmake_minimum_required(VERSION 3.10)
project(FAST_ANALYSIS
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 14)
# 변수에 값을 넣어주기

set(CMAKE_CXX_STANDARD_REQUIRED ON)
# C++ 17, 20 등의 기능이 사용되면 기능 제한

add_subdirectory(modules)

find_package(OpenCV REQUIRED)
find_package(easy_profiler REQUIRED)

link_libraries(${OpenCV_LIBS}
easy_profiler
)

include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(main exec/FAST_FILES.cpp)
add_executable(fast exec/Assignment.cpp)
add_executable(refac exec/refactoring_main.cpp exec/refactoring.cpp)
