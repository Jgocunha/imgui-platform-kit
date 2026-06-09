## Summary

<!-- One or two sentences describing WHAT this PR does and WHY. -->

## Type of Change

- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that changes existing behavior)
- [ ] Refactor / cleanup (no behavior change)
- [ ] Tests / coverage improvement
- [ ] Documentation update
- [ ] CI / build system change

## Platform(s) Affected

- [ ] Windows (DirectX 12)
- [ ] Linux (GLFW/OpenGL3)
- [ ] macOS (GLFW/OpenGL3)
- [ ] Platform-independent (shared code only)

## Related Issues

Closes #

## Changes Made

<!--
Bullet list of concrete changes. Include file paths and function names where relevant.
-->
-

## Testing Done

- [ ] Built on Windows
- [ ] Built on Linux
- [ ] Built on macOS
- [ ] Existing tests pass (`ctest --output-on-failure`)
- [ ] New tests added (describe below)
- [ ] Tested manually (describe below)

## Checklist

- [ ] Code follows the existing style (C++20, `imgui_kit` namespace)
- [ ] Public header documentation/comments updated if the API changed
- [ ] Tests added or updated for new/changed behavior
- [ ] Platform-specific code is correctly guarded (`#if defined(_WIN32)` / `#elif defined(__APPLE__)` / `#elif defined(__linux__)`)
- [ ] No new compiler warnings introduced
