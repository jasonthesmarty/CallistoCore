﻿# CMakeLists.txt for the CallistoCore project

cmake_minimum_required(VERSION 3.12)

project(CallistoCore VERSION 0.1)

set(SOURCES CallistoCore.cpp CallistoCore.ipp)

add_library(CallistoCore STATIC ${SOURCES})

target_compile_features(CallistoCore PUBLIC cxx_std_20)

find_package(Vulkan REQUIRED)

target_include_directories(CallistoCore PRIVATE ${CMAKE_SOURCE_DIR}\\dependencies\\incl)
target_include_directories(CallistoCore PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_directories(CallistoCore PRIVATE ${CMAKE_SOURCE_DIR}\\dependencies\\libs\\GLFW)
target_link_libraries(CallistoCore PRIVATE "glfw3.lib")
target_link_libraries(CallistoCore PRIVATE ${Vulkan_LIBRARIES})