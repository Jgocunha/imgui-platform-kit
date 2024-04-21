#pragma once

#include <cstring>

#include "user_interface_window.h"

namespace imgui_kit
{
	class LogWindow : public UserInterfaceWindow
	{
	private:
		inline static ImGuiTextBuffer	buffer;
		inline static ImGuiTextFilter	filter;
		inline static ImVector<int>		lineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		inline static ImVec4			textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // Default to white
		inline static bool				autoScroll = true;  // Keep scrolling if already at the bottom.
		inline static std::string		windowTitle;
		inline static bool 				isWindowActive = false;
	public:
		LogWindow();
		static void addLog(const char* message, ...) IM_FMTARGS(3);

		void render() override;

		static bool isActive();

		~LogWindow() override = default;
	private:
		static void clean();
		static void draw();
		static void getLogColor(const char* line_start);
		static void drawLog();
	};
}
