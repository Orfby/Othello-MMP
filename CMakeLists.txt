cmake_minimum_required(VERSION 3.14)
project(othello)

#C++14
set(CMAKE_CXX_STANDARD 14)

#Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/include)

#The source file directory
set(OTHELLO_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/othello")

#The source files
set(OTHELLO_SOURCE_FILES
        "${OTHELLO_SOURCE_DIR}/ai/AlphaBetaPruningPlayer.cpp"
        "${OTHELLO_SOURCE_DIR}/ai/MoveEvaluator.cpp"
        "${OTHELLO_SOURCE_DIR}/cmd/BoardPrinter.cpp"
        "${OTHELLO_SOURCE_DIR}/game/Board.cpp"
        "${OTHELLO_SOURCE_DIR}/game/Game.cpp"
        "${OTHELLO_SOURCE_DIR}/App.cpp"

        "${PROJECT_SOURCE_DIR}/src/main.cpp")

#A macro to setup the executable for a specific OS
macro(forOS os)
    #Make the bin/[os] folder
    file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${os}")

    #Create the othello executable
    add_executable(othello ${OTHELLO_SOURCE_FILES})

    #Output the executable into the bin/[os] folder
    set_target_properties(othello
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${os}"
            LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${os}"
            ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${os}")

    #If this is debug mode
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        #Add the debug option
        #target_compile_options(othello PUBLIC "-Og")
        target_compile_options(othello PUBLIC "-g")
    #If this is release mode
    else()
        #Add the optimization option
        target_compile_options(othello PUBLIC "-O1")
    endif()

    #If this is windows
    if (${os} STREQUAL "windows")
        #If this is debug mode
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            #Add the windows debug libraries
            target_link_libraries(othello
                    "${PROJECT_SOURCE_DIR}/libs/lib/windows/libboost_program_options-mgw81-mt-d-x64-1_72.a")
        #If this is release mode
        else()
            #Add the windows release libraries
            target_link_libraries(othello
                    "${PROJECT_SOURCE_DIR}/libs/lib/windows/libboost_program_options-mgw81-mt-x64-1_72.a")
        endif()
    else()
        target_link_libraries(othello
                "${PROJECT_SOURCE_DIR}/libs/lib/linux/libboost_program_options.a")
    endif()

endmacro()

#If we're on windows
if (WIN32)
    #Setup for windows
    forOS(windows)
#If we're on a mac
elseif (APPLE)
    #Not supported
    message("MacOS is not supported (yet!)")
#If we're on unix (but not mac)
elseif (UNIX)
    #Setup for linux
    forOS(linux)
endif()
