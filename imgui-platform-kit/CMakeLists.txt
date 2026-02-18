cmake_minimum_required(VERSION 3.15)

project(imgui-platform-kit
    DESCRIPTION "Cross-platform toolkit for Dear ImGui"
    LANGUAGES CXX)

# Define library version
set(IMGUI_PLATFORM_KIT_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(IMGUI_PLATFORM_KIT_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(IMGUI_PLATFORM_KIT_VERSION ${IMGUI_PLATFORM_KIT_VERSION_MAJOR}.${IMGUI_PLATFORM_KIT_VERSION_MINOR} CACHE STRING "version" FORCE)

# Library build type prefix
set(IMGUI_PLATFORM_KIT_LIB_PREFIX "release")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(IMGUI_PLATFORM_KIT_LIB_PREFIX "debug")
endif()

# Installation directories
set(IMGUI_PLATFORM_KIT_INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(IMGUI_PLATFORM_KIT_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(IMGUI_PLATFORM_KIT_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/${IMGUI_PLATFORM_KIT_LIB_PREFIX}")
set(IMGUI_PLATFORM_KIT_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/${IMGUI_PLATFORM_KIT_LIB_PREFIX}")
set(IMGUI_PLATFORM_KIT_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(IMGUI_PLATFORM_KIT_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}")
set(IMGUI_PLATFORM_KIT_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# VCPKG integration
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "ERROR: This project requires VCPKG.\n")
endif()

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Project directory definitions
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")
add_compile_definitions(PROJECT_DIR="${PROJECT_DIR}")

# Header and source files
set(common_header
    "include/user_interface_window.h"
    "include/user_interface_parameters.h"
    "include/template_window.h"
    "include/log_window.h"
    "include/stb_image.h"
    "include/user_interface.h"
    "include/colour_palette.h"
    "include/themes.h"
)

if(WIN32)
    set(platform_header
        "include/win32/win32_dx12_user_interface.h"
    )
elseif(UNIX AND NOT APPLE)
    set(platform_header
        "include/linux/glfw_opengl3_user_interface.h"
    )
endif()

set(header ${common_header} ${platform_header})

set(common_src
    "src/user_interface_window.cpp"
    "src/user_interface_parameters.cpp"
    "src/template_window.cpp"
    "src/log_window.cpp"
    "src/themes.cpp"
)

if(WIN32)
    set(platform_src
        "src/win32/win32_dx12_user_interface.cpp"
    )
elseif(UNIX AND NOT APPLE)
    set(platform_src
        "src/linux/glfw_opengl3_user_interface.cpp"
    )
endif()

set(src ${common_src} ${platform_src})

# Library target definition
add_library(${CMAKE_PROJECT_NAME} ${header} ${src})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${IMGUI_PLATFORM_KIT_INC_INSTALL_DIR}>
)

if(WIN32)
    # Setup imgui - win32 Directx12
    find_package(imgui CONFIG REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui "d3d12.lib" "dxgi.lib" "d3dcompiler.lib")
elseif(UNIX AND NOT APPLE)
    # Setup GLFW and OpenGL
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)

    # Setup imgui
    find_package(imgui CONFIG REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui glfw OpenGL)
endif()

# Setup implot
find_package(implot CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE implot::implot)

# Setup imgui-node-editor
find_package(unofficial-imgui-node-editor CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE unofficial::imgui-node-editor::imgui-node-editor)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    IMGUI_PLATFORM_KIT=1
    IMGUI_PLATFORM_KIT_VERSION_MAJOR=${IMGUI_PLATFORM_KIT_VERSION_MAJOR}
    IMGUI_PLATFORM_KIT_VERSION_MINOR=${IMGUI_PLATFORM_KIT_VERSION_MINOR}
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${IMGUI_PLATFORM_KIT_VERSION}"
    POSITION_INDEPENDENT_CODE ON
)

# Install the library (binaries)
install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
    RUNTIME DESTINATION ${IMGUI_PLATFORM_KIT_RUNTIME_INSTALL_DIR}
    LIBRARY DESTINATION ${IMGUI_PLATFORM_KIT_LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${IMGUI_PLATFORM_KIT_ARCHIVE_INSTALL_DIR}
    FRAMEWORK DESTINATION ${IMGUI_PLATFORM_KIT_FRAMEWORK_INSTALL_DIR}
)

# Install the headers (DIRECTORY preserves the internal directory structure)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${IMGUI_PLATFORM_KIT_INC_INSTALL_DIR}/imgui-platform-kit
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

# Install the headers (FILES installs only the files, without recreating their source folder paths)
# install(FILES ${header}
# DESTINATION ${IMGUI_PLATFORM_KIT_INC_INSTALL_DIR}/imgui-platform-kit)

# Package version configuration
include(CMakePackageConfigHelpers)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config-version.cmake")
write_basic_package_version_file(${version_file}
    VERSION ${IMGUI_PLATFORM_KIT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT ${CMAKE_PROJECT_NAME}Targets DESTINATION ${IMGUI_PLATFORM_KIT_CMAKE_CONFIG_INSTALL_DIR}
    FILE ${CMAKE_PROJECT_NAME}-config.cmake
)

# Example executable
set(EXE_PROJECT_EXAMPLE ${CMAKE_PROJECT_NAME}-example)
add_executable(${EXE_PROJECT_EXAMPLE} "examples/example.cpp")
target_include_directories(${EXE_PROJECT_EXAMPLE} PRIVATE include)
target_link_libraries(${EXE_PROJECT_EXAMPLE} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME})
