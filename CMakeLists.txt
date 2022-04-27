cmake_minimum_required(VERSION 3.22)

project(TesteCrossGui)

set(MAIN_SOURCES src/main.cpp)
set(MAIN_HEADERS src/includes.h)

set(GLAD_SOURCES submodules/glad/src/glad.c)

if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCES} ${MAIN_HEADERS})
else()
    add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})
    target_include_directories(${PROJECT_NAME} PRIVATE submodules/glad/src/include)
endif()

target_include_directories(${PROJECT_NAME} submodules/SDL/include)