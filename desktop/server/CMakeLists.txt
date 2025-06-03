cmake_minimum_required(VERSION 3.22)
project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)

set(SOURCES
    main.cpp
    src/window.cpp
    src/comservice.cpp
    src/tcpservice.cpp
    src/uartservice.cpp
)

# Tell CMake about the header files that need moc
set(HEADERS
    include/window.h
    include/comservice.h
    include/tcpservice.h
    include/uartservice.h
)

include_directories(include)

add_executable(server ${SOURCES} ${HEADERS})

target_link_libraries(server PRIVATE Qt6::Core Qt6::Widgets)
