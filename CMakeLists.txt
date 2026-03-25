cmake_minimum_required(VERSION 4.2.0)
project(Program)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 14)

find_package(wxWidgets REQUIRED COMPONENTS net core base)

if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

file(COPY original_data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} WIN32 main.cpp "program.rc")

target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

