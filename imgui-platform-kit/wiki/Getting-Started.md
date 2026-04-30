# Getting Started

## Prerequisites

| Requirement | Windows | Linux | macOS |
|---|---|---|---|
| C++20 compiler | MSVC 2019+ | GCC 10+ / Clang 12+ | Apple Clang 13+ (Xcode 13+) |
| CMake | 3.15+ | 3.15+ | 3.15+ |
| vcpkg | required | required | required |
| Xcode CLI Tools | — | — | `xcode-select --install` |

## 1. Clone the repository

```bash
git clone https://github.com/Jgocunha/imgui-platform-kit.git
cd imgui-platform-kit
```

## 2. Install vcpkg

**Windows:**
```bat
git clone https://github.com/microsoft/vcpkg.git %USERPROFILE%\vcpkg
%USERPROFILE%\vcpkg\bootstrap-vcpkg.bat -disableMetrics
setx VCPKG_ROOT "%USERPROFILE%\vcpkg"
```

**Linux:**
```bash
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh" -disableMetrics
export VCPKG_ROOT="$HOME/vcpkg"   # add to ~/.bashrc for persistence
```

**macOS:**
```bash
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh" -disableMetrics
export VCPKG_ROOT="$HOME/vcpkg"   # add to ~/.zshrc for persistence
```

## 3. Build

Run the script for your platform from inside the `imgui-platform-kit/` subdirectory:

**Windows:**
```bat
cd imgui-platform-kit
build.bat
```

**Linux:**
```bash
cd imgui-platform-kit
./build.sh
```

**macOS:**
```bash
cd imgui-platform-kit
./build_macos.sh
```

The macOS script auto-detects your architecture and selects the correct vcpkg triplet (`arm64-osx` for Apple Silicon, `x64-osx` for Intel).

## 4. Run the example

After a successful build, the example binary is at:

| Platform | Path |
|---|---|
| Windows | `build/windows-release/Release/imgui-platform-kit-example.exe` |
| Linux | `build/linux-release/imgui-platform-kit-example` |
| macOS | `build/macos-release/imgui-platform-kit-example` |

## 5. Integrate into your project

See [Building from Source](Building-from-Source) for CMake integration details and available build options.
