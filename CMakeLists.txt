cmake_minimum_required(VERSION 3.10)

project(TesteCrossGui)

set(TARGET_NAME ${PROJECT_NAME})

set(MAIN_SOURCES src/main.cpp)
set(MAIN_HEADERS src/includes.h)

set(GLAD_SOURCES submodules/glad/src/gl.c)

add_subdirectory(submodules/SDL)

if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(TARGET_NAME main)
    add_library(${TARGET_NAME} SHARED ${MAIN_SOURCES} ${MAIN_HEADERS})
    target_link_libraries(${TARGET_NAME} GLESv3 android EGL)
else()
    add_executable(${TARGET_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})
    target_include_directories(${TARGET_NAME} PRIVATE submodules/glad/include)
    #target_link_libraries(${TARGET_NAME} SDL2main)
endif()

target_include_directories(${TARGET_NAME} PRIVATE submodules/SDL/include)
target_link_libraries(${TARGET_NAME} SDL2 SDL2main)
