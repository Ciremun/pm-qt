# pm-qt

<img src="https://user-images.githubusercontent.com/38132413/117678109-c0fefd00-b1b7-11eb-8625-a67ae24e7674.png" alt="preview" width="540" height="360">

## Dependencies

[Qt5](https://www.qt.io/download-qt-installer)  
[CMake](https://cmake.org/download/)  
[SQLite3](https://www.sqlite.org/download.html)  

### Windows MSYS2:

    pacman -S mingw-w64-x86_64-sqlite3 mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-qt5-static

## Build

    mkdir build & cd build
    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ../ && cmake --build . --parallel
