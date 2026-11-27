
#pragma once

/**
 * @file glfw_opengl3_user_interface.h
 * @brief Linux backend of imgui_kit::UserInterface (GLFW + OpenGL 3).
 *
 * Include the platform-independent user_interface.h instead of this header;
 * it selects the right backend automatically. The public API is identical to
 * the documented Windows variant.
 */

#if defined(__linux__)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <memory>

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "implot.h"
#include "implot_internal.h"

#include "imgui-node-editor/imgui_node_editor.h"
namespace ImNodeEditor = ax::NodeEditor;


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


#include "../user_interface_parameters.h"
#include "../user_interface_window.h"

namespace imgui_kit
{
    /**
     * @brief OpenGL texture holding the background image and its parameters.
     * Managed internally by UserInterface.
     */
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

    /**
     * @brief Main application object (Linux GLFW + OpenGL 3 backend).
     *
     * Same public API as the documented Windows variant: initialize(),
     * render(), shutdown(), isShutdownRequested() and addWindow().
     */
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

        /**
         * @brief Creates the GLFW window and OpenGL context, sets up the ImGui and
         * ImPlot contexts, and applies the configured style, fonts, icon and
         * background image.
         *
         * Must be called once before render().
         * @throws std::runtime_error if GLFW cannot be initialized (for example,
         * when there is no display) or cannot create the window.
         */
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