cmake_minimum_required(VERSION 3.26)
project(lab4 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab4 main.c
        queue.c
        queue.h
        engine.c
        engine.h
        util.c
        util.h)

enable_testing()

add_test(
        NAME case1
        COMMAND $<TARGET_FILE:lab4> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case1.txt
)

add_test(
        NAME case2
        COMMAND $<TARGET_FILE:lab4> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case2.txt
)

add_test(
        NAME case3
        COMMAND $<TARGET_FILE:lab4> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case3.txt
)

add_test(
        NAME case4
        COMMAND $<TARGET_FILE:lab4> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case4.txt
)