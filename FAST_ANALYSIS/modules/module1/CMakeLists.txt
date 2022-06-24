cmake_minimum_required(VERSION 3.11)
project(module1)

set(MODULE1_SOURCE_FILES
    src/module1.cpp
    )


add_library(module1
    ${MODULE1_SOURCE_FILES}
    )

target_include_directories(module1 PUBLIC
    include)