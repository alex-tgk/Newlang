cmake_minimum_required(VERSION 3.16)
project(Newlang C)

set(CMAKE_C_STANDARD 99)

add_executable(Newlang src/main.c src/lexer.c src/lexer.h src/input.c src/input.h src/parser.c src/parser.h)