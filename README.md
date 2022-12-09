<img src="img/demo.gif">

## Requirements

In order to successfully build the application, you need to have `CMake` on your machine. Furthermore, you need to have installed `mingw32-make`, if you are on Windows, or `g++`, if you are on Linux. 

### Build

### Dependencies

`CMake` automatically pulls down additional dependencies from `GitHub` that required by the application.

- https://github.com/Perlmint/glew-cmake.git
- https://github.com/glfw/glfw
- https://github.com/ocornut/imgui
- https://github.com/epezent/implot
- https://github.com/gabime/spdlog
- https://github.com/nlohmann/json


```
cmake -S . -B build/release -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
```
### Linux

```
make -C build/release
```

```
./build/release/src/covid19_sim misc/fonts/Cousine-Regular.ttf
```

### Windows

```
mingw32-make -C build\release
```

```
build\release\src\covid19_sim.exe misc\fonts\Cousine-Regular.ttf
```
