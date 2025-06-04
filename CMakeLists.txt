cmake_minimum_required(VERSION 3.22)

project(av24tr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_library(client_gui STATIC ./desktop/client/src/window.cpp ./desktop/client/src/canvas.cpp)
target_include_directories(client_gui PRIVATE ./desktop/client/include ./shared)
target_link_libraries(client_gui PRIVATE Qt6::Core Qt6::Widgets)

add_executable(client_app desktop/client/main.cpp)
target_include_directories(client_app PRIVATE ./desktop/client/include)
target_link_libraries(client_app PRIVATE client_gui Qt6::Core Qt6::Widgets)

set(SERVER_GUI_SOURCES
    ./desktop/server/src/window.cpp
)

set(SERVER_GUI_HEADERS
    ./desktop/server/include/window.h
)

add_library(server_gui STATIC
    ${SERVER_GUI_SOURCES}
    ${SERVER_GUI_HEADERS}
)

target_include_directories(server_gui PRIVATE ./desktop/server/include ./shared)
target_link_libraries(server_gui PRIVATE Qt6::Core Qt6::Widgets)



add_executable(server_app desktop/server/main.cpp)
target_include_directories(server_app PRIVATE ./desktop/server/include)
target_link_libraries(server_app PRIVATE server_gui)
target_link_libraries(server_app PRIVATE Qt6::Widgets)
target_link_libraries(server_app PRIVATE Qt6::Core)


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