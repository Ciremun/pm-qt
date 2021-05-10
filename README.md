# pm-qt

![preview](https://user-images.githubusercontent.com/38132413/117666187-47154680-b1ac-11eb-8faf-ba1c9b6b80e5.png)

## Build

### Windows MSYS2:

CMake generator: `-G"MinGW Makefiles"`  
If `make` in PATH: `-G"Unix Makefiles"`  

```console
mkdir build & cd build
cmake -DCMAKE_BUILD_TYPE=Release ../ && cmake --build . --parallel
```
