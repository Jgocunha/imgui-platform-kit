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
// #include <filesystem>
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
		class UserInterface
		{
		private:
			UserInterfaceParameters parameters;
			//DX12BackgroundImageTexture backgroundImageTexture;
			//HWND windowHandle;
			//WNDCLASSEXW windowClass;
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

// // Dear ImGui stuff
// struct FrameContext
// {
// 	ID3D12CommandAllocator* CommandAllocator;
// 	UINT64                  FenceValue;
// };

// // Data
// static int constexpr NUM_FRAMES_IN_FLIGHT = 3;
// static FrameContext g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
// static UINT g_frameIndex = 0;
// static int constexpr NUM_BACK_BUFFERS = 3;
// static ID3D12Device* g_pd3dDevice = nullptr;
// static ID3D12DescriptorHeap* g_pd3dRtvDescHeap = nullptr;
// static ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
// static ID3D12CommandQueue* g_pd3dCommandQueue = nullptr;
// static ID3D12GraphicsCommandList* g_pd3dCommandList = nullptr;
// static ID3D12Fence* g_fence = nullptr;
// static HANDLE g_fenceEvent = nullptr;
// static UINT64 g_fenceLastSignaledValue = 0;
// static IDXGISwapChain3* g_pSwapChain = nullptr;
// static HANDLE g_hSwapChainWaitableObject = nullptr;
// static ID3D12Resource* g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
// static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

// // Forward declarations of helper functions
// bool CreateDeviceD3D(HWND hWnd);
// void CleanupDeviceD3D();
// void CreateRenderTarget();
// void CleanupRenderTarget();
// void WaitForLastSubmittedFrame();
// FrameContext* WaitForNextFrameResources();
// LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height);