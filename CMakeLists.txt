cmake_minimum_required(VERSION 3.25)
project(file_integrity_check)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

set(SOURCE
        implementation/main.cpp
        implementation/file_check_sum_calculator.cpp
        implementation/utils/args_parser.cpp
        implementation/utils/utils.cpp
        implementation/utils/sys_logger.cpp
        implementation/calculate_queue.cpp
        implementation/timer/calculate_timer.cpp
        implementation/signal_handler/signal_handler.cpp
        )

set(HEADERS
        implementation/file_check_sum_calculator.hpp
        implementation/utils/args_parser.hpp
        implementation/utils/utils.hpp
        implementation/utils/sys_logger.hpp
        implementation/calculate_queue.hpp
        implementation/timer/calculate_timer.hpp
        implementation/signal_handler/signal_handler.hpp
        )

add_executable(file_integrity_check ${SOURCE} ${HEADERS})

target_link_libraries(file_integrity_check Boost::headers pthread)

