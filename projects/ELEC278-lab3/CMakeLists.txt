cmake_minimum_required(VERSION 3.20)
project(lab3 C)

set(CMAKE_C_STANDARD 11)

add_executable(linked-stack linked-stack.c)
add_executable(array-stack array-stack.c)

add_library(demo-impl OBJECT
        demo.c
        demo.h
        main.c
)

target_link_libraries(linked-stack demo-impl)
target_link_libraries(array-stack demo-impl)

enable_testing()

add_test(
        NAME array-case1
        COMMAND $<TARGET_FILE:array-stack> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case1.txt
)

add_test(
        NAME array-case2
        COMMAND $<TARGET_FILE:array-stack> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case2.txt fail
)

add_test(
        NAME array-case3
        COMMAND $<TARGET_FILE:array-stack> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case3.txt fail
)

add_test(
        NAME linked-case1
        COMMAND $<TARGET_FILE:linked-stack> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case1.txt
)

add_test(
        NAME linked-case2
        COMMAND $<TARGET_FILE:linked-stack> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case2.txt fail
)

add_test(
        NAME linked-case3
        COMMAND $<TARGET_FILE:linked-stack> ${CMAKE_CURRENT_LIST_DIR}/test-cases/case3.txt fail
)
