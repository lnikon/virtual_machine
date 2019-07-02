cmake_minimum_required(VERSION 3.13)
project(virtual_machine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O0 -Wall ")
set(CMAKE_CXX_STANDARD 17)

set(EXEC_NAME "czlp")

set(SOURCES main.cpp
            defs.hpp
            debug.hpp
            common.hpp
            utility.hpp
            utility.cpp
            array.hpp
            extension_defs.hpp
            function.hpp
            instruction.hpp
            instruction_envirnoment.hpp
            instruction_envirnoment.cpp
            label.hpp
            operand.hpp
            type_defs.hpp
            variable.hpp
            variable.cpp
            vm.cpp
            vm.hpp
            vm_config.hpp
            lexer.cpp
            lexer.hpp
            lexer_defs.hpp
            parser.cpp
            parser.hpp
            stack_section_parser.hpp
            stack_section_parser.cpp
            stack_section_parser_utilities.hpp
            stack_section_parser_utilities.cpp
            data_section_parser.cpp
            data_section_parser.hpp
            data_section_parser_utilities.hpp
            data_section_parser_utilities.cpp
            code_section_parser.cpp
            code_section_parser.hpp
            parser_defs.hpp
            logger.cpp
            logger.hpp)

add_executable(${EXEC_NAME} ${SOURCES})
