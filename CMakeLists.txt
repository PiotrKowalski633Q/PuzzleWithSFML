cmake_minimum_required(VERSION 3.25)
project(PuzzleWithSFML)

set(CMAKE_CXX_STANDARD 23)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/Build)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "Dependencies/SFML-2.6.0/lib/cmake/SFML")

file(COPY Resources DESTINATION ${dir}/Build)
file(COPY Dependencies/SFML-2.6.0/bin/openal32.dll DESTINATION ${dir}/Build)

find_package(SFML 2.6.0 REQUIRED COMPONENTS audio graphics window system)

include_directories(Dependencies/SFML-2.6.0/include)


add_executable(PuzzleWithSFML
        Code/Sources/main.cpp
        Code/Headers/Piece.h
        Code/Headers/Board.h
        Code/Sources/Piece.cpp
        Code/Sources/Board.cpp)
target_link_libraries(PuzzleWithSFML sfml-audio sfml-graphics sfml-window sfml-system)