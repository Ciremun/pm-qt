cmake_minimum_required(VERSION 3.0.0)
project(pm-qt VERSION 0.0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_WIN32_EXECUTABLE OFF)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(SQLite3)

add_executable(${PROJECT_NAME} src/pm-qt.cpp src/aes.c src/b64/encode.c src/b64/decode.c src/b64/buffer.c)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES})