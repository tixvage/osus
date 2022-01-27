cmake_minimum_required(VERSION 3.0)
project (OSUS)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/include)



set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)
set(source_dir "${PROJECT_SOURCE_DIR}")

file(GLOB_RECURSE source_files "${source_dir}/*.cpp")

set(PROJECT_INCLUDE_DIR
    ${PROJECT_DIR}/include
	${PROJECT_DIR}/libs/raylib/src
)

include_directories(${PROJECT_INCLUDE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/build/raylib/desktop/)
add_executable(${PROJECT_NAME} ${source_files})
if (WIN32)
	target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 pthread winmm)
endif (WIN32)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} raylib dl pthread)
endif (UNIX)

target_link_libraries(${PROJECT_NAME} stdc++fs)