# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed
- On Linux and macOS, `UserInterface::initialize()` now throws `std::runtime_error` when GLFW cannot be initialized (`"Failed to initialize GLFW."`) or cannot create the window (`"Failed to create the GLFW window."`). It used to return with no window and no ImGui context, so the next `render()` segfaulted on the null window. This crashed dynamic-neural-field-composer's headless example smoke-run whenever the X connection was refused.

---

## [2.1.0] — 2026-11-26

### Added
- Pre-filled issue backlog in `.github/issues/` documenting 5 known bugs and 7 planned features.
- Gemini-powered issue triage workflow (`gemini-issue-triage.yml`): auto-labels and welcomes new issues, detects duplicates, and validates model output before auto-closing.
- Documentation sync workflow (`gemini-doc-sync.yml`): audits Doxygen, README, and CHANGELOG on PRs touching `include/`; updates existing comment in place on re-runs.
- Monthly vcpkg maintenance workflow (`vcpkg-maintenance.yml`): creates a version report issue on the 1st of each month for `imgui`, `implot`, `imgui-node-editor`, `glfw3` and `catch2`.
- `.coderabbit.yaml`: CodeRabbit auto-review with C++20 style, platform guard, Doxygen, and test coverage rules.

### Changed
- Dependencies are declared in a vcpkg manifest, `imgui-platform-kit/vcpkg.json`, and pinned by its `builtin-baseline` to vcpkg commit `0699a19d0c` (imgui 1.92.9, implot 1.0, imgui-node-editor 0.9.3, glfw3 3.5.1, catch2 3.16.0). The first configure installs them into `<build tree>/vcpkg_installed/`; the build scripts no longer run `vcpkg install` (`build.sh` no longer needs `sudo` for it). Previously every build took whatever vcpkg HEAD offered, so an upstream release could change a build that changed nothing. `VCPKG_ROOT` must now be a git clone of vcpkg at the baseline commit or newer; a vcpkg unpacked from a release tarball, as the README used to suggest for Linux, no longer works. Delete existing `build/` directories once: a tree configured before the manifest existed keeps vcpkg's classic mode cached.
- CI: runner images pinned (`ubuntu-24.04`, `windows-2025-vs2026`, `macos-26`) instead of `*-latest`; vcpkg cloned at the pinned baseline; vcpkg's binary cache is cached instead of its `installed/` and `packages/` trees. The monthly vcpkg report lists each package's pinned version next to the latest one.
- The library itself is unchanged; projects consuming it through `find_package(imgui-platform-kit)` need no changes.

### Fixed
- CMakeLists version bumped from `1.0` to `2.0.0`; added `VERSION_PATCH` variable and compile definition to match the v2.0.0 git tag.
- The CMakeLists no longer requires `VCPKG_ROOT` in the environment when vcpkg's toolchain is passed as `CMAKE_TOOLCHAIN_FILE`; it only falls back to `VCPKG_ROOT` when no vcpkg toolchain was given. vcpkg clears `VCPKG_ROOT` for port builds on Windows, so the kit could not be built as a vcpkg port there without a workaround in the portfile.
- `build.sh` tested the literal string `'$VCPKG_ROOT'`, so it never stopped when the variable was unset. `build.sh` and `build.bat` exited with code 1 even after a successful build, and `build_macos.sh` exited with 0 after a failed one; all three now exit with the build's result.

---

## [2.0.1] — 2026-06-09

### Added
- `CHANGELOG.md` following the Keep a Changelog format.
- GitHub issue templates (`bug_report.yml`, `feature_request.yml`) as structured YAML forms with platform-aware dropdowns and automatic labeling (`bug`/`enhancement`, `needs-triage`).
- GitHub PR template (`PULL_REQUEST_TEMPLATE.md`) with per-platform build checklists and a preprocessor guard reminder.
- `config.yml` to disable blank issues and provide upstream contact links (Dear ImGui, ImPlot, imgui-node-editor).

### Changed
- Wiki documentation reorganized and expanded with new pages: Getting Started, Configuration, Creating Windows, Built-in Windows, Themes and Colours, ImPlot and Node Editor, Building from Source.

---

## [2.0.0] — 2026-04-30

### Added
- Initial stable release of imgui-platform-kit.
- Cross-platform support: Windows (DirectX 12), Linux (GLFW/OpenGL3), macOS (GLFW/OpenGL3).
- `UserInterface` class with `initialize()`, `render()`, `shutdown()`, and `addWindow<T>()`.
- `UserInterfaceParameters` configuration system: `WindowParameters`, `FontParameters`, `StyleParameters`, `IconParameters`, `BackgroundImageParameters`.
- `UserInterfaceWindow` abstract base class for custom windows.
- Built-in `LogWindow` with colored, filterable, auto-scrolling log output (`addLog`, `clearLogs`, `getLogCount`).
- `TemplateWindow` as a minimal subclassing example.
- 39 built-in themes via `Theme` enum and `showImGuiKitThemeSelector()`.
- 50+ color constants in `colour_palette.h` (Basic, Extended, Grayscale, Pastel, Dark groups).
- Global window flags management (`getGlobalWindowFlags`, `setGlobalWindowFlags`, `addGlobalWindowFlags`, `removeGlobalWindowFlags`).
- ImPlot integration: context created/destroyed automatically; exposed via `user_interface_window.h`.
- imgui-node-editor integration: linked and context destroyed on shutdown.
- `stb_image.h` bundled for image loading.
- Background image support with fit modes (DX12 and OpenGL paths).
- DPI scaling on all three platforms.
- JetBrainsMono font family bundled (Regular, Bold, Italic, Light, Medium, Thin).
- Window icon loading: `.ico` on Windows, `.png` on Linux (macOS: GLFW limitation, no-op).
- VCPKG-based dependency management with CMake 3.15+ and C++20.
- GitHub Actions CI workflows for Windows, Linux, and macOS builds.
- Code coverage workflow (Linux/GCC, lcov, Codecov upload).
- Release packaging workflow.
- Unit tests with Catch2: window flags, colour palette, themes, parameters, log window.
- CMake install targets with config exports for downstream `find_package` use.
- Build and install scripts for all platforms (`build.bat`, `build.sh`, `build_macos.sh`, `install.bat`, `install.sh`).
- SRV descriptor heap management for DX12 background image textures.
- Retry logic for vcpkg dependency installation in CI.
- `--ignore-errors unused` option for lcov to improve coverage reporting robustness.
- macOS architecture auto-detection in CI (arm64-osx for Apple Silicon, x64-osx for Intel).

### Fixed
- `IM_FMTARGS` index in `LogWindow::addLog` declaration.
- CMake install prefix paths for Windows, Linux, and macOS.
- GLFW window creation error handling.
- `UserInterfaceParameters` test to avoid GLFW monitor query in headless CI.
- Catch2 installation step ordering in Windows and macOS CI workflows.
- Redundant test cases removed; `colour_palette.h` included in all test files.
- `log_window`: replaced `<stdarg.h>` with `<cstdarg>` for standard conformance.

### Changed
- `LogWindow::renderContent()` extracted from the main render path for modularity.
- Global window flag variables converted from global to `inline constexpr` for better linkage.
- SRV descriptor heap management refactored into dedicated helper functions in the DX12 backend.

[Unreleased]: https://github.com/Jgocunha/imgui-platform-kit/compare/v2.1.0...HEAD
[2.1.0]: https://github.com/Jgocunha/imgui-platform-kit/compare/v2.0.1...v2.1.0
[2.0.1]: https://github.com/Jgocunha/imgui-platform-kit/compare/v2.0.0...v2.0.1
[2.0.0]: https://github.com/Jgocunha/imgui-platform-kit/releases/tag/v2.0.0