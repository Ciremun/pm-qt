# pm-qt

## Build

### Windows MSYS2:

CMake generator: `-G"MinGW Makefiles"`  
If `make` in PATH: `-G"Unix Makefiles"`  

```console
mkdir build & cd build
cmake -DCMAKE_BUILD_TYPE=Release ../ && cmake --build . --parallel
```