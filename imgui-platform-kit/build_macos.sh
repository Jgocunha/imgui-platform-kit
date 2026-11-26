#!/bin/bash

export PROJECT_ROOT=$(pwd)

echo "VCPKG_ROOT is set to: '$VCPKG_ROOT'"

if [ -z "$VCPKG_ROOT" ]; then
    echo "ERROR: The environment variable VCPKG_ROOT is not set."
    echo "Download and install vcpkg from https://github.com/microsoft/vcpkg"
    echo "Set VCPKG_ROOT to point to your vcpkg installation directory."
    read -p "Press any key to continue... " -n1 -s
    exit 1
fi

# Detect architecture for vcpkg triplet
ARCH=$(uname -m)
if [ "$ARCH" = "arm64" ]; then
    VCPKG_TRIPLET="arm64-osx"
else
    VCPKG_TRIPLET="x64-osx"
fi

echo "Using vcpkg triplet: $VCPKG_TRIPLET"

# Dependencies are declared in vcpkg.json and installed by CMake on the first configure.

# Create and enter build directory
mkdir -p "$PROJECT_ROOT/build/macos-release"
cd "$PROJECT_ROOT/build/macos-release"

cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET="$VCPKG_TRIPLET" \
    -DCMAKE_BUILD_TYPE=Release

make -j$(sysctl -n hw.logicalcpu)

read -p "Press any key to continue... " -n1 -s
exit 0
