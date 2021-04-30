cmake_minimum_required(VERSION 3.0.0)
project(pm-qt VERSION 0.0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
# if(WIN32)
#     set(CMAKE_WIN32_EXECUTABLE ON)
# endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PM_QT_SOURCES src/aes.c src/b64/encode.c src/b64/decode.c src/b64/buffer.c src/rand.c 
src/main.cpp src/app.cpp src/encrypt.cpp src/common.cpp)

set(SOURCES ${PM_SOURCES} ${PM_QT_SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)