# Building from Source

## CMake options

| Option | Default | Description |
|---|---|---|
| `IMGUI_PLATFORM_KIT_BUILD_TESTS` | `ON` | Build the Catch2 test suite |
| `IMGUI_PLATFORM_KIT_COVERAGE` | `OFF` | Instrument with `--coverage` (GCC/Clang only) |

## Manual CMake configure + build

### Windows

```bat
cmake -S . -B build/windows-release ^
    -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET=x64-windows ^
    -DCMAKE_BUILD_TYPE=Release

cmake --build build/windows-release --config Release --parallel
```

### Linux

```bash
cmake -S . -B build/linux-release \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=x64-linux \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build/linux-release --parallel
```

### macOS

```bash
# Apple Silicon
cmake -S . -B build/macos-release \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=arm64-osx \
    -DCMAKE_BUILD_TYPE=Release

# Intel
cmake -S . -B build/macos-release \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=x64-osx \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build/macos-release --parallel
```

## vcpkg triplets

| Platform | Architecture | Triplet |
|---|---|---|
| Windows | x64 | `x64-windows` |
| Linux | x64 | `x64-linux` |
| macOS | Apple Silicon (ARM64) | `arm64-osx` |
| macOS | Intel (x86-64) | `x64-osx` |

## Running the tests

```bash
cd build/<platform>
ctest --output-on-failure
```

## Coverage build (Linux, GCC or Clang)

```bash
cmake -S . -B build/coverage \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=x64-linux \
    -DCMAKE_BUILD_TYPE=Debug \
    -DIMGUI_PLATFORM_KIT_COVERAGE=ON

cmake --build build/coverage --parallel
cd build/coverage
ctest --output-on-failure

lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' "$HOME/vcpkg/*" '*/tests/*' '*/examples/*' \
     --output-file coverage.info --ignore-errors unused
lcov --list coverage.info
```

The `IMGUI_PLATFORM_KIT_COVERAGE` CMake option enables `--coverage` for both GCC and Clang. The `lcov` commands above use GCC's `gcov` by default. With Clang, pass `--gcov-tool llvm-cov gcov` to `lcov`, or use `llvm-cov show` directly. The CI coverage workflow runs on GCC (`ubuntu-latest`).

## Installing

After building, install with:

```bash
cmake --install build/<platform> --prefix /path/to/install
```

Or use the provided scripts:

```bat
install.bat   # Windows
```
```bash
./install.sh  # Linux
```

Then in your consumer project:

```cmake
find_package(imgui-platform-kit REQUIRED)
target_link_libraries(MyApp PRIVATE imgui-platform-kit)
```
