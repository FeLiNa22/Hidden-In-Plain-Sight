cmake_minimum_required(VERSION 3.14)

project(Stenography C)

set(CMAKE_CXX_STANDARD 14)

include_directories('inc/')

add_executable(Stenography src/bmp.c src/main.c src/encode.c src/decode.c src/utils.c src/encrypt.c src/encrypt_utils.c)


