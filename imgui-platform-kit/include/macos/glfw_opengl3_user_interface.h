
#pragma once

#if defined(__APPLE__)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <filesystem>

#include <stdio.h>
// Silence OpenGL deprecation warnings on macOS (OpenGL deprecated since 10.14 but still functional)
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "implot.h"
#include "implot_internal.h"

#include "imgui-node-editor/imgui_node_editor.h"
namespace ImNodeEditor = ax::NodeEditor;

#include "../user_interface_parameters.h"
#include "../user_interface_window.h"

namespace imgui_kit
{
    struct GLFWbackgroundImageTexture
    {
        GLuint texture;
        BackgroundImageParameters parameters;

        GLFWbackgroundImageTexture()
        : texture(0)
        , parameters()
        {}

        explicit GLFWbackgroundImageTexture(BackgroundImageParameters parameters)
        : texture(0), parameters(std::move(parameters))
        {}
    };

    class UserInterface
    {
    private:
        UserInterfaceParameters parameters;
        GLFWbackgroundImageTexture backgroundImageTexture;
        GLFWwindow* window;
        std::vector<std::shared_ptr<UserInterfaceWindow>> windows;
        bool shutdownRequest;
    public:
        UserInterface();
        explicit UserInterface(UserInterfaceParameters parameters);
        ~UserInterface() = default;

        void initialize();
        void render();
        void shutdown() const;
        [[nodiscard]] bool isShutdownRequested() const;
        template<typename T, typename... Args>
        void addWindow(Args&&... args)
        {
            auto window = std::make_shared<T>(std::forward<Args>(args)...);
            windows.push_back(std::move(window));
        }
    private:
        void loadIcon() const;
        void loadFont();
        void loadBackgroundImage();
        void renderWindows() const;
        void renderBackgroundImage() const;
        void updateLastRenderedFrameDimensions();
        void updateFontGlobalScale() const;
    };

}

// Forward declarations of helper functions
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
float GetDpiScale(GLFWwindow* window);
GLFWmonitor* GetActiveMonitor(GLFWwindow* window);
#endif
