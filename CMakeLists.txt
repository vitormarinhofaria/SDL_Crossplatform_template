cmake_minimum_required(VERSION 3.10)

project(TesteCrossGui)

set(TARGET_NAME ${PROJECT_NAME})

set(MAIN_SOURCES src/main.cpp)
set(MAIN_HEADERS src/includes.h)

set(GLAD_SOURCES submodules/glad/src/gl.c)

add_subdirectory(submodules/SDL)

set(IMGUI_SRC_PATH submodules/imgui)
set(IMGUI_SRCS ${IMGUI_SRC_PATH}/imgui_demo.cpp ${IMGUI_SRC_PATH}/imgui_draw.cpp ${IMGUI_SRC_PATH}/imgui_tables.cpp 
               ${IMGUI_SRC_PATH}/imgui_widgets.cpp ${IMGUI_SRC_PATH}/imgui.cpp)

set(IMGUI_IMPL_SRCS ${IMGUI_SRC_PATH}/backends/imgui_impl_opengl3.cpp ${IMGUI_SRC_PATH}/backends/imgui_impl_sdl.cpp)

if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(TARGET_NAME main)
    add_library(${TARGET_NAME} SHARED ${MAIN_SOURCES} ${MAIN_HEADERS} ${IMGUI_SRCS} ${IMGUI_IMPL_SRCS})
    target_link_libraries(${TARGET_NAME} GLESv3 android EGL SDL2)
else()
    add_executable(${TARGET_NAME}     ${MAIN_SOURCES} ${MAIN_HEADERS} ${IMGUI_SRCS} ${IMGUI_IMPL_SRCS} ${GLAD_SOURCES})
    target_include_directories(${TARGET_NAME} PRIVATE submodules/glad/include)
    target_link_libraries(${TARGET_NAME} SDL2-static)
endif()

target_include_directories(${TARGET_NAME} PRIVATE submodules/SDL/include ${IMGUI_SRC_PATH} ${IMGUI_SRC_PATH}/backends)
target_link_libraries(${TARGET_NAME} SDL2main)
