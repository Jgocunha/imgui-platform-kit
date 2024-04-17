#pragma once

#include "imgui.h"
// #include "imgui_impl_win32.h"
// #include "imgui_impl_dx12.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// #include <d3d12.h>
// #include <dxgi1_4.h>
#include <string>
// #include <tchar.h>
// #include <stdexcept>
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

// #ifdef _DEBUG
// #define DX12_ENABLE_DEBUG_LAYER
// #endif

// #ifdef DX12_ENABLE_DEBUG_LAYER
// #include <dxgidebug.h>
// #pragma comment(lib, "dxguid.lib")
// #endif

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


#include "user_interface_parameters.h"
#include "user_interface_window.h"

namespace imgui_kit
{
	namespace glfw_opengl
	{
		struct GLFWbackgroundImageTexture
		{
			GLuint texture;
			BackgroundImageParameters parameters;

			GLFWbackgroundImageTexture() 
			: texture(0)
			, parameters() 
			{}

			GLFWbackgroundImageTexture(BackgroundImageParameters parameters)
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
			UserInterface(UserInterfaceParameters parameters);
			~UserInterface() = default;

			void initialize();
			void render();
			void shutdown();
			bool isShutdownRequested() const;
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
		};
	}
}

// Forward declarations of helper functions
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
