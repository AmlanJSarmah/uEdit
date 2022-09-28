cmake_minimum_required(VERSION 3.0)

project(uEdit)

include_directories(${PROJECT_SOURCE_DIR}/headers)

add_executable(${PROJECT_NAME} main.c terminal.c utils.c)
