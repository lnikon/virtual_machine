cmake_minimum_required(VERSION 3.13)
project(virtual_machine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wall")
set(CMAKE_CXX_STANDARD 17)

set(EXEC_NAME "czlp")

set(SOURCES main.cpp
            defs.hpp
            common.hpp
            lexer.cpp
            lexer.hpp
            parser.cpp
            parser.hpp
            logger.cpp
            logger.hpp)

add_executable(${EXEC_NAME} ${SOURCES})
