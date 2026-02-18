
#pragma once

#include <vector>
#include <string>
#include <cstdarg>
#include "imgui.h"
#include "colour_palette.h"
#include "user_interface_window.h"

namespace imgui_kit
{
    struct LogEntry
	{
        std::string message;
        ImVec4 color;
    };

    class LogWindow final : public UserInterfaceWindow
	{
    private:
        inline static std::vector<LogEntry> logs;
        inline static ImGuiTextFilter filter;
        inline static bool autoScroll = true;
        inline static bool isWindowActive = false;

    public:
        LogWindow();
        static void addLog(const ImVec4& color, const char* fmt, ...) IM_FMTARGS(3);
        void render() override { draw(); }
        static bool isActive() { return isWindowActive;}
        static void renderContent();
        ~LogWindow() override = default;
    private:
        static void clean() { logs.clear(); }
        static void draw();
    };
}
