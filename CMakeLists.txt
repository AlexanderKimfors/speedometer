cmake_minimum_required(VERSION 3.22)

project(av24tr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)



set(COMM_PROTOCOL "TCP" CACHE STRING "Communication protocol (TCP|UART)")
set_property(CACHE COMM_PROTOCOL PROPERTY STRINGS TCP UART)

set(PROTO_MACRO "")
set(PROTO_QT_COMPONENT "")
set(CLIENT_PROTO_SRC "")
set(SERVER_PROTO_SRC "")

if (COMM_PROTOCOL STREQUAL "TCP")
    set(PROTO_MACRO COMM_PROTO_TCP)
    set(PROTO_QT_COMPONENT Network)
    set(CLIENT_PROTO_SRC desktop/client/src/tcpservice.cpp)
    set(SERVER_PROTO_SRC desktop/server/src/tcpservice.cpp)
elseif (COMM_PROTOCOL STREQUAL "UART")
    set(PROTO_MACRO COMM_PROTO_UART)
    set(PROTO_QT_COMPONENT SerialPort)
    set(CLIENT_PROTO_SRC desktop/client/src/uartservice.cpp)
    set(SERVER_PROTO_SRC desktop/server/src/uartservice.cpp)
else()
    message(FATAL_ERROR "Unknown COMM_PROTOCOL='${COMM_PROTOCOL}'. Use TCP or UART.")
endif()

foreach(f ${CLIENT_PROTO_SRC} ${SERVER_PROTO_SRC})
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${f}")
        message(FATAL_ERROR "Expected source '${f}' for protocol ${COMM_PROTOCOL} not found.")
    endif()
endforeach()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets ${PROTO_QT_COMPONENT})

add_executable(client
    desktop/client/main.cpp
    desktop/client/src/window.cpp
    desktop/client/src/canvas.cpp
    desktop/client/src/comservice.cpp
    ${CLIENT_PROTO_SRC}
)
target_include_directories(client PRIVATE ./desktop/client/include ./shared)
target_link_libraries(client PRIVATE Qt6::Core Qt6::Widgets Qt6::${PROTO_QT_COMPONENT})
target_compile_definitions(client PRIVATE ${PROTO_MACRO})

add_executable(server
    desktop/server/main.cpp
    desktop/server/src/window.cpp
    desktop/server/src/comservice.cpp
    ${SERVER_PROTO_SRC}
)
target_include_directories(server PRIVATE ./desktop/server/include ./shared)
target_link_libraries(server PRIVATE Qt6::Core Qt6::Widgets Qt6::${PROTO_QT_COMPONENT})
target_compile_definitions(server PRIVATE ${PROTO_MACRO})

#---------------------------------------------------------------------------------------------
set(FONT_NAME MaterialIcons.ttf)
set(SOURCE_FONT_PATH "${CMAKE_SOURCE_DIR}/desktop/client/res/${FONT_NAME}")
set(DEST_FONT_DIR "$ENV{HOME}/.local/share/fonts")
set(DEST_FONT_PATH "${DEST_FONT_DIR}/${FONT_NAME}")

if(NOT EXISTS "${DEST_FONT_PATH}")
    message(STATUS "Font saknas – installerar till: ${DEST_FONT_PATH}")
    file(MAKE_DIRECTORY "${DEST_FONT_DIR}")
    file(COPY "${SOURCE_FONT_PATH}" DESTINATION "${DEST_FONT_DIR}")
    execute_process(
        COMMAND fc-cache -f -v "${DEST_FONT_DIR}"
        RESULT_VARIABLE FC_RESULT
        OUTPUT_VARIABLE FC_OUTPUT
        ERROR_VARIABLE FC_ERROR
    )
    if(FC_RESULT EQUAL 0)
        message(STATUS "fc-cache uppdaterad:\n${FC_OUTPUT}")
    else()
        message(WARNING "fc-cache misslyckades:\n${FC_ERROR}")
    endif()
else()
    message(STATUS "Fonten finns redan: ${DEST_FONT_PATH}")
endif()
#---------------------------------------------------------------------------------------------

