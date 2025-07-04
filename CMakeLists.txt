cmake_minimum_required(VERSION 3.22)

project(speedometer LANGUAGES CXX)

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
    )
target_include_directories(client PRIVATE desktop/client/include shared)
target_link_libraries(client PRIVATE Qt6::Widgets)

add_executable(server
    desktop/server/main.cpp
    desktop/server/src/window.cpp
    )
target_include_directories(server PRIVATE desktop/server/include shared)
target_link_libraries(server PRIVATE Qt6::Widgets)

if(NOT EXISTS "$ENV{HOME}/.local/share/fonts/MaterialIcons.ttf")
    message(STATUS "Font saknas - installeras till: $ENV{HOME}/.local/share/fonts/")
    file(MAKE_DIRECTORY "$ENV{HOME}/.local/share/fonts") # Skapar mappen om den inte redan finns
    file(COPY "${CMAKE_SOURCE_DIR}/desktop/client/res/MaterialIcons.ttf" DESTINATION "$ENV{HOME}/.local/share/fonts") # Kopierar filen i projektet stället som OS leter i
    execute_process(
        COMMAND fc-cache -f -v "${DEST_FONT_DIR}"
        RESULT_VARIABLE FC_RESULT
    )    
    if(FC_RESULT EQUAL 0)
        message(STATUS "fc-cache uppdaterad:\n${FC_OUTPUT}")
    else()
        message(WARNING "fc-cache misslyckades:\n${FC_ERROR}")
    endif()
else()
    message(STATUS "Fonten finns redan: ${DEST_FONT_PATH}")
endif()