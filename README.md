# pm-qt

![preview](https://user-images.githubusercontent.com/38132413/117666187-47154680-b1ac-11eb-8faf-ba1c9b6b80e5.png)

## Dependencies

[Qt5](https://www.qt.io/download-qt-installer)
[CMake](https://cmake.org/download/)
[SQLite3](https://www.sqlite.org/download.html)

### Windows MSYS2:

    pacman -S mingw-w64-x86_64-sqlite3 mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-qt5-static

## Build

    mkdir build & cd build
    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ../ && cmake --build . --parallel
