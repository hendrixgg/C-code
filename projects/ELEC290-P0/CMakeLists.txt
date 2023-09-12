cmake_minimum_required(VERSION 3.27)
project(problems C)

set(CMAKE_C_STANDARD 11)

add_executable(strategy_compare strategy_compare.c strategies.c)
