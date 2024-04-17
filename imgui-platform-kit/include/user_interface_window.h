#pragma once

#include "imgui.h"
//#include "imgui_impl_win32.h"
#include "imgui_impl_glfw.h"
//#include "imgui_impl_dx12.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"
#include "implot_internal.h"

// #include <d3d12.h>
// #include <dxgi1_4.h>
 #include <string>
// #include <tchar.h>
// #include <stdexcept>
// #include <iostream>
// #include <vector>

#include "implot.h"
#include "implot_internal.h"

// #ifdef _DEBUG
// #define DX12_ENABLE_DEBUG_LAYER
// #endif

// #ifdef DX12_ENABLE_DEBUG_LAYER
// #include <dxgidebug.h>
// #pragma comment(lib, "dxguid.lib")
// #endif


namespace imgui_kit
{
	struct UserInterfaceWindowParameters
	{
		// This struct is intentionally left empty.
		// Derived structs are expected to define specific member variables
		// and functions relevant to their respective elements.
		UserInterfaceWindowParameters() = default;
		virtual ~UserInterfaceWindowParameters() = default;
	};

	class UserInterfaceWindow
	{
	public:
		UserInterfaceWindow() = default;
		virtual void render() = 0;
		virtual ~UserInterfaceWindow() = default;

		UserInterfaceWindow(const UserInterfaceWindow&) = delete;
		UserInterfaceWindow& operator=(const UserInterfaceWindow&) = delete;
		UserInterfaceWindow(UserInterfaceWindow&&) = delete;
		UserInterfaceWindow& operator=(UserInterfaceWindow&&) = delete;
	};
}