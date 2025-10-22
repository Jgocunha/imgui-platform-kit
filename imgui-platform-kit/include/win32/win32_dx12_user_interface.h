#pragma once

#if defined(_WIN32)

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <string>
#include <tchar.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <filesystem>

#include "implot.h"
#include "implot_internal.h"

#include <imgui-node-editor/imgui_node_editor.h>


#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#include "user_interface_parameters.h"
#include "user_interface_window.h"

namespace imgui_kit
{
	struct DX12BackgroundImageTexture
	{
		ID3D12Resource* texture;
		D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle{};
		D3D12_GPU_DESCRIPTOR_HANDLE srv_gpu_handle{};
		BackgroundImageParameters parameters;

		DX12BackgroundImageTexture()
			: texture(nullptr)
			,parameters()
		{
			srv_cpu_handle.ptr = 0;
			srv_gpu_handle.ptr = 0;
		}

		explicit DX12BackgroundImageTexture(BackgroundImageParameters parameters)
			: texture(nullptr), parameters(std::move(parameters))
		{
			srv_cpu_handle.ptr = 0;
			srv_gpu_handle.ptr = 0;
		}

		void release()
		{
			if (texture)
				texture->Release();
			texture = nullptr;
		}

		~DX12BackgroundImageTexture()
		{
			if (texture)
				texture->Release();
		}
	};

	class UserInterface
	{
	private:
		UserInterfaceParameters parameters;
		DX12BackgroundImageTexture backgroundImageTexture;
		HWND windowHandle;
		WNDCLASSEXW windowClass;
		std::vector<std::shared_ptr<UserInterfaceWindow>> windows;
		bool shutdownRequest;
	public:
		UserInterface();
		explicit UserInterface(UserInterfaceParameters parameters);
		~UserInterface() = default;

		void initialize();
		void render();
		void shutdown();
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
	};
}

// Dear ImGui stuff
struct FrameContext
{
	ID3D12CommandAllocator* CommandAllocator;
	UINT64                  FenceValue;
};

// Data
inline int constexpr NUM_FRAMES_IN_FLIGHT = 3;
inline FrameContext g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
inline UINT g_frameIndex = 0;
inline int constexpr NUM_BACK_BUFFERS = 3;
inline ID3D12Device* g_pd3dDevice = nullptr;
inline ID3D12DescriptorHeap* g_pd3dRtvDescHeap = nullptr;
inline ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
inline ID3D12CommandQueue* g_pd3dCommandQueue = nullptr;
inline ID3D12GraphicsCommandList* g_pd3dCommandList = nullptr;
inline ID3D12Fence* g_fence = nullptr;
inline HANDLE g_fenceEvent = nullptr;
inline UINT64 g_fenceLastSignaledValue = 0;
inline IDXGISwapChain3* g_pSwapChain = nullptr;
inline HANDLE g_hSwapChainWaitableObject = nullptr;
inline ID3D12Resource* g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
inline D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void WaitForLastSubmittedFrame();
FrameContext* WaitForNextFrameResources();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height);
std::wstring StringToWString(const std::string& str);
float GetDpiScale(HWND hWnd);

#endif