cmake_minimum_required(VERSION 3.22)

project(av24tr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(client
    desktop/client/main.cpp
    desktop/client/src/window.cpp
    desktop/client/src/canvas.cpp
    desktop/client/src/comservice.cpp
    desktop/client/src/tcpservice.cpp
    )
target_include_directories(client PRIVATE ./desktop/client/include ./shared)
target_link_libraries(client PRIVATE client_gui Qt6::Core Qt6::Widgets)


add_executable(server
    desktop/server/main.cpp
    desktop/server/src/window.cpp
    desktop/server/src/comservice.cpp
    desktop/server/src/tcpservice.cpp
    )
target_include_directories(server PRIVATE ./desktop/server/include ./shared)
target_link_libraries(server PRIVATE server_gui Qt6::Core Qt6::Widgets)


#---------------------------------------------------------------------------------------------

set(FONT_NAME MaterialIcons.ttf)
set(SOURCE_FONT_PATH "${CMAKE_SOURCE_DIR}/desktop/client/res/${FONT_NAME}")
set(DEST_FONT_DIR "$ENV{HOME}/.local/share/fonts")
set(DEST_FONT_PATH "${DEST_FONT_DIR}/${FONT_NAME}")# Kolla om fonten redan finns
if(NOT EXISTS "${DEST_FONT_PATH}")
    message(STATUS "Font saknas – installerar till: ${DEST_FONT_PATH}")    # Skapa katalogen om den inte finns
    file(MAKE_DIRECTORY "${DEST_FONT_DIR}")    # Kopiera fontfilen
    file(COPY "${SOURCE_FONT_PATH}" DESTINATION "${DEST_FONT_DIR}")    # Kör fc-cache
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
endif()#---------------------------------------------------------------------------------------------