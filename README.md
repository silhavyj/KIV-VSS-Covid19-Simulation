### Build

```
cmake -S . -B build/release -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make -C build/release
```

### Run

```
./build/release/src/covid19_sim misc/fonts/Cousine-Regular.ttf
```
