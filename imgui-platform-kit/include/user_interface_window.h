#pragma once

#include <string>

#include "imgui.h"

#if defined(_WIN32)
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#elif defined(__linux__) || defined(__APPLE__)
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

#include "implot.h"
#include "implot_internal.h"

#include <imgui-node-editor/imgui_node_editor.h>
namespace ImNodeEditor = ax::NodeEditor;

namespace imgui_kit
{
	extern ImGuiWindowFlags g_globalWindowFlags;
	ImGuiWindowFlags getGlobalWindowFlags();
	void setGlobalWindowFlags(ImGuiWindowFlags flags);
	void addGlobalWindowFlags(ImGuiWindowFlags flags);
	void removeGlobalWindowFlags(ImGuiWindowFlags flags);

	struct UserInterfaceWindowParameters final
	{
		// This struct is intentionally left empty.
		// Derived structs are expected to define specific member variables
		// and functions relevant to their respective elements.
		UserInterfaceWindowParameters() = default;
		~UserInterfaceWindowParameters() = default;
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