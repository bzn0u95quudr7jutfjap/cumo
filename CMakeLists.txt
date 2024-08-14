cmake_minimum_required(VERSION 3.5)
project(cumo)

set(sources
)

include_directories(inc/)

add_executable(${PROJECT_NAME} src/main.c ${sources})
