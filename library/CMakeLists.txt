cmake_minimum_required(VERSION 3.27)
project(HGC_Library C)


add_executable(main tests.c hgc_library.c hgc_library_tests.c)