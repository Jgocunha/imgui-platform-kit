# imgui-platform-kit Wiki

**ImGui Platform Kit** is a cross-platform C++ toolkit that wraps [Dear ImGui](https://github.com/ocornut/imgui), [ImPlot](https://github.com/epezent/implot), and [imgui-node-editor](https://github.com/thedmd/imgui-node-editor) into a single, easy-to-integrate library. It handles platform setup, rendering backends, DPI scaling, font loading, and window management so you can focus on your UI logic.

## Platform support

| Platform | Backend | Status |
|---|---|---|
| Windows | Win32 + DirectX 12 | Supported |
| Linux | GLFW + OpenGL 3 | Supported |
| macOS | GLFW + OpenGL 3 | Supported (OpenGL 3.2 Core) |

## Pages

| Page | Description |
|---|---|
| [Getting Started](Getting-Started) | Clone, install prerequisites, build, run the example |
| [Configuration](Configuration) | `UserInterfaceParameters` and all sub-parameter structs |
| [Creating Windows](Creating-Windows) | Subclassing `UserInterfaceWindow`, the render loop, `addWindow<T>()` |
| [Built-in Windows](Built-in-Windows) | `LogWindow` usage and the log API |
| [Themes and Colours](Themes-and-Colours) | Available themes, `ColourPalette`, `StyleParameters` |
| [ImPlot and Node Editor](ImPlot-and-Node-Editor) | Using ImPlot and imgui-node-editor inside your windows |
| [Building from Source](Building-from-Source) | CMake options, vcpkg triplets, coverage builds |
| [Contributing](Contributing) | Branch conventions, CI workflows, adding a new platform backend |
