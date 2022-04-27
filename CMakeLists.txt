cmake_minimum_required(VERSION 3.22)

project(TesteCrossGui)

set(MAIN_SOURCES src/main.cpp)
set(MAIN_HEADERS src/includes.h)

set(GLAD_SOURCES submodules/glad/src/gl.c)

add_subdirectory(submodules/SDL)

if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCES} ${MAIN_HEADERS})
else()
    add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})
    target_include_directories(${PROJECT_NAME} PRIVATE submodules/glad/include)
    target_link_libraries(${PROJECT_NAME} SDL2main)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE submodules/SDL/include)
target_link_libraries(${PROJECT_NAME} SDL2-static)
