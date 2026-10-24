cmake_minimum_required(VERSION 3.16)
project(FetchTheGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window Audio System Network)

add_executable(FetchTheGame src/main.cpp 
src/GameEng.cpp 
src/Menu.cpp 
src/Dog.cpp 
src/Ball.cpp
src/SkinSelector.h
src/resource.h
src/GameMenu.h
src/GameLogic.h
src/GameEng.h
src/Dog.h
src/Ball.h
)

target_link_libraries(FetchTheGame PRIVATE sfml-graphics sfml-window sfml-audio sfml-system sfml-network)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fontforgame.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DogMenu.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ButtonSound.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR})