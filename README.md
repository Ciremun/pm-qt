# pm-qt

**NOT FINISHED** ![reeHop](https://cdn.betterttv.net/emote/608d1cbc39b5010444d0994e/1x)

## Build

### Windows MSYS2:

CMake generator: -G"MinGW Makefiles"  
If `make` in PATH: -G"Unix Makefiles"  

```console
mkdir build & cd build
cmake -DCMAKE_BUILD_TYPE=Release ../ && cmake --build . --parallel
```