#pragma once

#if defined(_WIN32)
#include "win32/win32_dx12_user_interface.h"
#elif defined(__APPLE__)
#include "macos/glfw_opengl3_user_interface.h"
#elif defined(__linux__)
#include "linux/glfw_opengl3_user_interface.h"
#endif