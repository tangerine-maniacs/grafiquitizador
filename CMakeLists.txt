cmake_minimum_required(VERSION 3.14)
project(grafiquitizador VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME} main.c bmpwriter.h bmpwriter.c)