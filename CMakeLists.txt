cmake_minimum_required(VERSION 3.22)
project(av24tr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# ---------------- paths ----------------
set(CLIENT_DIR desktop/client)
set(SERVER_DIR desktop/server)
set(RES_DIR    ${CLIENT_DIR}/res)
set(FONTS_DIR  "$ENV{HOME}/.local/share/fonts")

set(SERVER_UPORT "/dev/ttyUSB0" CACHE STRING "Server UART port")
set(CLIENT_UPORT "/dev/ttyUSB1" CACHE STRING "Client UART port")

# ---------------- Qt components (GLOBAL) ----------------
set(QT_PROTO_COMPONENT_TCP  "Network"    CACHE STRING "Qt6 component for TCP")
set(QT_PROTO_COMPONENT_UART "SerialPort" CACHE STRING "Qt6 component for UART")

set(QT_PROTO_COMPONENT "${QT_PROTO_COMPONENT_TCP}")
if(UARTCOM)
  set(QT_PROTO_COMPONENT "${QT_PROTO_COMPONENT_UART}")
endif()

set(QT_COMPONENTS Core Widgets ${QT_PROTO_COMPONENT})
find_package(Qt6 REQUIRED COMPONENTS ${QT_COMPONENTS})


# ---------------- client/server lists ----------------
set(CLIENT_SOURCES
  ${CLIENT_DIR}/src/canvas.cpp
  ${CLIENT_DIR}/src/comservice.cpp
  ${CLIENT_DIR}/src/window.cpp
  ${CLIENT_DIR}/main.cpp
)

set(SERVER_SOURCES
  ${SERVER_DIR}/src/comservice.cpp
  ${SERVER_DIR}/src/window.cpp
  ${SERVER_DIR}/main.cpp
)

set(CLIENT_TCP_SOURCES   ${CLIENT_DIR}/src/tcpservice.cpp)
set(SERVER_TCP_SOURCES   ${SERVER_DIR}/src/tcpservice.cpp)

set(CLIENT_UART_SOURCES  ${CLIENT_DIR}/src/uartservice.cpp)
set(SERVER_UART_SOURCES  ${SERVER_DIR}/src/uartservice.cpp)


# ----- (UART or TCP) -----
if(UARTCOM)
  add_compile_definitions(UARTCOM UART_SPORT="${SERVER_UPORT}" UART_CPORT="${CLIENT_UPORT}")
  set(CLIENT_SOURCES ${CLIENT_SOURCES} ${CLIENT_UART_SOURCES})
  set(CLIENT_LIBRARIES Qt6::Core Qt6::Widgets ${PROTO_QT_TARGET})

  set(SERVER_SOURCES ${SERVER_SOURCES} ${SERVER_UART_SOURCES})
  set(SERVER_LIBRARIES Qt6::Core Qt6::Widgets ${PROTO_QT_TARGET})
else()
  set(CLIENT_SOURCES ${CLIENT_SOURCES} ${CLIENT_TCP_SOURCES})
  set(CLIENT_LIBRARIES Qt6::Core Qt6::Widgets ${PROTO_QT_TARGET})

  set(SERVER_SOURCES ${SERVER_SOURCES} ${SERVER_TCP_SOURCES})
  set(SERVER_LIBRARIES Qt6::Core Qt6::Widgets ${PROTO_QT_TARGET})
endif()

# ---------------- targets ----------------
add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PUBLIC shared ${CLIENT_DIR}/include)
target_link_libraries(client PUBLIC Qt6::Core Qt6::Widgets Qt6::${QT_PROTO_COMPONENT})

add_executable(server ${SERVER_SOURCES})
target_include_directories(server PUBLIC shared ${SERVER_DIR}/include)
target_link_libraries(server PUBLIC Qt6::Core Qt6::Widgets Qt6::${QT_PROTO_COMPONENT})

if(UARTCOM)
  target_compile_definitions(client PRIVATE UARTCOM UART_SPORT="${SERVER_UPORT}" UART_CPORT="${CLIENT_UPORT}")
  target_compile_definitions(server PRIVATE UARTCOM UART_SPORT="${SERVER_UPORT}" UART_CPORT="${CLIENT_UPORT}")
endif()

# ---------------- fonts  ----------------
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
# --------------------------------------------------------------------------------------------

add_custom_target(use_uart
  COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR} -DUARTCOM=ON
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target client server --parallel
  USES_TERMINAL
  COMMENT "Switching to UART (UARTCOM=ON) and rebuilding client+server"
)

add_custom_target(use_tcp
  COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR} -DUARTCOM=OFF
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target client server --parallel
  USES_TERMINAL
  COMMENT "Switching to TCP (UARTCOM=OFF) and rebuilding client+server"
)
# --------------------------------------------------------------------------------------------

