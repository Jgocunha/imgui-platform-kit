
#if defined(_WIN32)

#include "user_interface.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace imgui_kit
{
        UserInterface::UserInterface()
	        : parameters()
            , backgroundImageTexture()
            , windowHandle(nullptr)
            , windowClass({})
            , shutdownRequest(false)
        {}

		UserInterface::UserInterface(UserInterfaceParameters parameters)
			: parameters(std::move(parameters))
			, backgroundImageTexture(parameters.backgroundImageParameters)
			, windowHandle(nullptr)
			, windowClass({}) 
			, shutdownRequest(false)
		{}

		void UserInterface::initialize()
		{
            const std::wstring lpszWindowName = StringToWString(parameters.windowParameters.title);

			ImGui_ImplWin32_EnableDpiAwareness();
			windowClass = { sizeof(windowClass), CS_CLASSDC, WndProc, 0L, 0L,
				GetModuleHandle(nullptr), nullptr, nullptr, nullptr,
				nullptr, lpszWindowName.c_str(), nullptr};
			::RegisterClassExW(&windowClass);

			windowHandle = ::CreateWindowW(windowClass.lpszClassName, lpszWindowName.c_str(),
				WS_OVERLAPPEDWINDOW, parameters.windowParameters.startPosX, parameters.windowParameters.startPosY, parameters.windowParameters.width, 
                parameters.windowParameters.height, nullptr, nullptr, windowClass.hInstance, nullptr);

			// Initialize Direct3D
            shutdownRequest = false;
			if (!CreateDeviceD3D(windowHandle))
			{
				CleanupDeviceD3D();
				::UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
                //shutdownRequest = true;
			}

			// Show the window
			::ShowWindow(windowHandle, SW_SHOWDEFAULT);
			::UpdateWindow(windowHandle);

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImPlot::CreateContext();

			ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;
			io_ref.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io_ref.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io_ref.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
			io_ref.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Platform/Renderer backends
			ImGui_ImplWin32_Init(windowHandle);
			ImGui_ImplDX12_InitInfo dx12InitInfo;
			dx12InitInfo.Device = g_pd3dDevice;
			dx12InitInfo.CommandQueue = g_pd3dCommandQueue;
			dx12InitInfo.NumFramesInFlight = NUM_FRAMES_IN_FLIGHT;
			dx12InitInfo.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			dx12InitInfo.SrvDescriptorHeap = g_pd3dSrvDescHeap;
			dx12InitInfo.SrvDescriptorAllocFn = ImGui_ImplDX12_SrvDescAlloc;
			dx12InitInfo.SrvDescriptorFreeFn = ImGui_ImplDX12_SrvDescFree;
			ImGui_ImplDX12_Init(&dx12InitInfo);

            loadIcon();
			loadFont();
            loadBackgroundImage();
            parameters.styleParameters.apply();
		}

		void UserInterface::render()
		{
			// Poll and handle messages (inputs, window resize, etc.)
		   // See the WndProc() function below for our to dispatch events to the Win32 backend.
			MSG msg;
            shutdownRequest = false;
			while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
                    shutdownRequest = true;
			}

			if (shutdownRequest)
				return;

			// Start the Dear ImGui frame
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

            renderBackgroundImage();
            renderWindows();
            
			ImGui::Render();

			FrameContext* frameCtx = WaitForNextFrameResources();
			const UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
			frameCtx->CommandAllocator->Reset();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			g_pd3dCommandList->Reset(frameCtx->CommandAllocator, nullptr);
			g_pd3dCommandList->ResourceBarrier(1, &barrier);

			// Render Dear ImGui graphics
        	const float bgColor[4] =
                { parameters.styleParameters.windowBgColor.x * parameters.styleParameters.windowBgColor.w,
                    parameters.styleParameters.windowBgColor.y * parameters.styleParameters.windowBgColor.w,
                    parameters.styleParameters.windowBgColor.z * parameters.styleParameters.windowBgColor.w,
                    parameters.styleParameters.windowBgColor.w };
        	g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], bgColor, 0, nullptr);
			g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
			g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			g_pd3dCommandList->ResourceBarrier(1, &barrier);
			g_pd3dCommandList->Close();

			g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);


            const ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;
			// Update and Render additional Platform Windows
			if (io_ref.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault(nullptr, (void*)g_pd3dCommandList);
			}

			g_pSwapChain->Present(1, 0); // Present with vsync
			//g_pSwapChain->Present(0, 0); // Present without vsync

			const UINT64 fenceValue = g_fenceLastSignaledValue + 1;
			g_pd3dCommandQueue->Signal(g_fence, fenceValue);
			g_fenceLastSignaledValue = fenceValue;
			frameCtx->FenceValue = fenceValue;

            updateLastRenderedFrameDimensions();
		}

		void UserInterface::shutdown()
		{
			WaitForLastSubmittedFrame();

            parameters.save();
            backgroundImageTexture.release();

			// Cleanup
            ImNodeEditor::DestroyEditor(ImNodeEditor::GetCurrentEditor());
			ImGui_ImplDX12_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			CleanupDeviceD3D();
			::DestroyWindow(windowHandle);
			::UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
		}

        bool UserInterface::isShutdownRequested() const
        {
	        return shutdownRequest;
        }

        void UserInterface::loadIcon() const
		{
            const HICON hIcon = static_cast<HICON>(LoadImage(
                nullptr,                        // hInstance must be NULL when loading from a file
                parameters.iconParameters.path.c_str(),    // Path to the icon file
                IMAGE_ICON,                     // Specifies that the file is an icon
                parameters.iconParameters.width, parameters.iconParameters.height, // Use the actual size of the icon if 0, 0
                LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_LOADTRANSPARENT
            ));

            if (hIcon != nullptr)
            {
                SendMessage(windowHandle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
                SendMessage(windowHandle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
            }
            else
            {
                const DWORD error = GetLastError();
                std::cerr << "Failed to load icon. Error code: " << error << std::endl;
            }
		}

        void UserInterface::loadFont()
        {
            ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;

            bool fontLoaded = false;

            for (const auto& [path, size] : parameters.fontParameters.pathsAndSizes)
            {
                // Replace forward slashes with backslashes
                std::string adjustedPath = path;
                size_t pos;
                while ((pos = adjustedPath.find('/')) != std::string::npos)
                    adjustedPath.replace(pos, 1, "\\");

                // Check if the font file exists
                if (!std::filesystem::exists(adjustedPath))
                {
                    std::cerr << "Font file does not exist: " << adjustedPath << std::endl;
                    continue; // Try the next font in the list
                }

                // Attempt to load the font
                const ImFont* font = io_ref.Fonts->AddFontFromFileTTF(adjustedPath.c_str(), static_cast<float>(size));
                if (font == nullptr)
                {
                    std::cerr << "Failed to load font: " << adjustedPath << " with size " << size << std::endl;
                    continue; // Try the next font in the list
                }

                fontLoaded = true; // At least one font was loaded successfully
            }

            // If no fonts were loaded, add a default font
            if (!fontLoaded)
            {
                std::cerr << "No fonts loaded successfully. Adding default font." << std::endl;
                io_ref.Fonts->AddFontDefault();
            }

            // Set global scale
            io_ref.FontGlobalScale = GetDpiScale(windowHandle);
        }

        void UserInterface::loadBackgroundImage()
        {
			// We need to pass a D3D12_CPU_DESCRIPTOR_HANDLE in ImTextureID, so make sure it will fit
			static_assert(sizeof(ImTextureID) >= sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), "D3D12_CPU_DESCRIPTOR_HANDLE is too large to fit in an ImTextureID");

			// We presume here that we have our D3D device pointer in g_pd3dDevice
			// Get CPU/GPU handles for the shader resource view
			// Normally your engine will have some sort of allocator for these - here we assume that there's an SRV descriptor heap in
			// g_pd3dSrvDescHeap with at least two descriptors allocated, and descriptor 1 is unused
			constexpr int descriptor_index = 0; // slot 0 is reserved for background image
			backgroundImageTexture.srv_cpu_handle = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
			backgroundImageTexture.srv_cpu_handle.ptr += (g_SrvDescriptorSize * descriptor_index);
			backgroundImageTexture.srv_gpu_handle = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();
			backgroundImageTexture.srv_gpu_handle.ptr += (g_SrvDescriptorSize * descriptor_index);
			// Load the texture from a file
			const bool ret = LoadTextureFromFile(parameters.backgroundImageParameters.path.c_str(),
			 g_pd3dDevice, 
			 backgroundImageTexture.srv_cpu_handle,
			 &backgroundImageTexture.texture,
			 &backgroundImageTexture.parameters.width,
			 &backgroundImageTexture.parameters.height);
			if (!ret)
				std::cerr << "Failed to load background image." << std::endl;
		}

		void UserInterface::renderWindows() const
		{
            for (const auto& window : windows)
                window->render();
		}

        void UserInterface::renderBackgroundImage() const
        {
            // Obtain the main viewport
            ImGuiViewport* viewport = ImGui::GetMainViewport();

            // Use the viewport's size to determine how to center the image
            const ImVec2 windowSize = viewport->Size; // This is the size of the area we can draw in

            const float dpiScale = GetDpiScale(windowHandle);

            // Original image size scaled by DPI
            const ImVec2 originalImageSize = ImVec2(
                (float)backgroundImageTexture.parameters.width * dpiScale,
                (float)backgroundImageTexture.parameters.height * dpiScale);

            // Calculate the aspect ratio of the image
            const float aspectRatio = originalImageSize.x / originalImageSize.y;

            ImVec2 imageSize;
            switch (parameters.backgroundImageParameters.fitType)
            {
            case ImageFitType::KEEP_ASPECT_RATIO:
            {
                if (windowSize.x / aspectRatio <= windowSize.y) // Fit to width
                {
                    imageSize.x = windowSize.x;
                    imageSize.y = windowSize.x / aspectRatio;
                }
                else // Fit to height
                {
                    imageSize.y = windowSize.y;
                    imageSize.x = windowSize.y * aspectRatio;
                }
            }
            break;
            case ImageFitType::ZOOM_TO_FIT:
            {
                if (windowSize.x / aspectRatio > windowSize.y) // Cover height
                {
                    imageSize.x = windowSize.x;
                    imageSize.y = windowSize.x / aspectRatio;
                }
                else // Cover width
                {
                    imageSize.y = windowSize.y;
                    imageSize.x = windowSize.y * aspectRatio;
                }
            }
            break;
            }

            // Calculate the top-left position to center the image in the viewport
            const ImVec2 pos = ImVec2(viewport->Pos.x + (windowSize.x - imageSize.x) * 0.5f,
                viewport->Pos.y + (windowSize.y - imageSize.y) * 0.5f);

            // Get the viewport's background draw list and add the image
            ImGui::GetBackgroundDrawList(viewport)->AddImage(
                (ImTextureID)backgroundImageTexture.srv_gpu_handle.ptr,
                pos, ImVec2(pos.x + imageSize.x, pos.y + imageSize.y));
        }

		void UserInterface::updateLastRenderedFrameDimensions()
		{
            RECT rect;
            if (GetWindowRect(windowHandle, &rect))
            {
                parameters.windowParameters.width = rect.right - rect.left;
                parameters.windowParameters.height = rect.bottom - rect.top;
                parameters.windowParameters.startPosX = rect.left;
                parameters.windowParameters.startPosY = rect.top;
            }
            else
            {
                const DWORD error = GetLastError();
                std::cerr << "GetWindowRect failed with error: " << error << std::endl;
            }
		}
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC1 sd;
    {
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = NUM_BACK_BUFFERS;
        sd.Width = 0;
        sd.Height = 0;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.Stereo = FALSE;
    }

    // [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
    ID3D12Debug* pdx12Debug = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
        pdx12Debug->EnableDebugLayer();
#endif

    // Create device
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    if (D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
        return false;

    // [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
    if (pdx12Debug != nullptr)
    {
        ID3D12InfoQueue* pInfoQueue = nullptr;
        g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
        pInfoQueue->Release();
        pdx12Debug->Release();
    }
#endif

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = NUM_BACK_BUFFERS;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
            return false;

        SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        {
            g_mainRenderTargetDescriptor[i] = rtvHandle;
            rtvHandle.ptr += rtvDescriptorSize;
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = SRV_HEAP_SIZE;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
            return false;

        g_SrvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        g_SrvFreeSlots.clear();
        for (UINT i = 1; i < SRV_HEAP_SIZE; i++) // slot 0 reserved for background image
            g_SrvFreeSlots.push_back(i);
    }

    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
            return false;
    }

    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
            return false;

    if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
        g_pd3dCommandList->Close() != S_OK)
        return false;

    if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
        return false;

    g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (g_fenceEvent == nullptr)
        return false;

    {
        IDXGIFactory4* dxgiFactory = nullptr;
        IDXGISwapChain1* swapChain1 = nullptr;
        if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
            return false;
        if (dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue, hWnd, &sd, nullptr, nullptr, &swapChain1) != S_OK)
            return false;
        if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
            return false;
        swapChain1->Release();
        dxgiFactory->Release();
        g_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
        g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
    }

    CreateRenderTarget();
    return true;
}

void ImGui_ImplDX12_SrvDescAlloc(ImGui_ImplDX12_InitInfo* /*info*/, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu)
{
    IM_ASSERT(!g_SrvFreeSlots.empty() && "SRV descriptor heap is full!");
    UINT slot = g_SrvFreeSlots.back();
    g_SrvFreeSlots.pop_back();
    out_cpu->ptr = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart().ptr + slot * g_SrvDescriptorSize;
    out_gpu->ptr = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart().ptr + slot * g_SrvDescriptorSize;
}

void ImGui_ImplDX12_SrvDescFree(ImGui_ImplDX12_InitInfo* /*info*/, D3D12_CPU_DESCRIPTOR_HANDLE cpu, D3D12_GPU_DESCRIPTOR_HANDLE /*gpu*/)
{
    UINT slot = static_cast<UINT>((cpu.ptr - g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart().ptr) / g_SrvDescriptorSize);
    g_SrvFreeSlots.push_back(slot);
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    g_SrvFreeSlots.clear();
    if (g_pSwapChain) { g_pSwapChain->SetFullscreenState(false, nullptr); g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_hSwapChainWaitableObject != nullptr) { CloseHandle(g_hSwapChainWaitableObject); }
    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_frameContext[i].CommandAllocator) { g_frameContext[i].CommandAllocator->Release(); g_frameContext[i].CommandAllocator = nullptr; }
    if (g_pd3dCommandQueue) { g_pd3dCommandQueue->Release(); g_pd3dCommandQueue = nullptr; }
    if (g_pd3dCommandList) { g_pd3dCommandList->Release(); g_pd3dCommandList = nullptr; }
    if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap->Release(); g_pd3dRtvDescHeap = nullptr; }
    if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap->Release(); g_pd3dSrvDescHeap = nullptr; }
    if (g_fence) { g_fence->Release(); g_fence = nullptr; }
    if (g_fenceEvent) { CloseHandle(g_fenceEvent); g_fenceEvent = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }

#ifdef DX12_ENABLE_DEBUG_LAYER
    IDXGIDebug1* pDebug = nullptr;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
    {
        pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
        pDebug->Release();
    }
#endif
}

void CreateRenderTarget()
{
    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
    {
        ID3D12Resource* pBackBuffer = nullptr;
        g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, g_mainRenderTargetDescriptor[i]);
        g_mainRenderTargetResource[i] = pBackBuffer;
    }
}

void CleanupRenderTarget()
{
    WaitForLastSubmittedFrame();

    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = nullptr; }
}

void WaitForLastSubmittedFrame()
{
    FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue == 0)
        return; // No fence was signaled

    frameCtx->FenceValue = 0;
    if (g_fence->GetCompletedValue() >= fenceValue)
        return;

    g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
    WaitForSingleObject(g_fenceEvent, INFINITE);
}

FrameContext* WaitForNextFrameResources()
{
    UINT nextFrameIndex = g_frameIndex + 1;
    g_frameIndex = nextFrameIndex;

    HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, nullptr };
    DWORD numWaitableObjects = 1;

    FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue != 0) // means no fence was signaled
    {
        frameCtx->FenceValue = 0;
        g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
        waitableObjects[1] = g_fenceEvent;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return frameCtx;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            WaitForLastSubmittedFrame();
            CleanupRenderTarget();
            HRESULT result = g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
            //assert(SUCCEEDED(result) && "Failed to resize swapchain.");
            CreateRenderTarget();
        }
        return 0;
    case WM_DPICHANGED:
        if (g_pd3dDevice != nullptr)
		{
            ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;
            io_ref.FontGlobalScale = GetDpiScale(hWnd);
		}
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
	default: break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// Simple helper function to load an image into a DX12 texture with common settings
// Returns true on success, with the SRV CPU handle having an SRV for the newly-created texture placed in it (srv_cpu_handle must be a handle in a valid descriptor heap)
bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture resource
    D3D12_HEAP_PROPERTIES props;
    memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
    props.Type = D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = image_width;
    desc.Height = image_height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* pTexture = NULL;
    d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
        D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

    // Create a temporary upload resource to move the data in
    UINT uploadPitch = (image_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
    UINT uploadSize = image_height * uploadPitch;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = uploadSize;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    props.Type = D3D12_HEAP_TYPE_UPLOAD;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    ID3D12Resource* uploadBuffer = NULL;
    HRESULT hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
    IM_ASSERT(SUCCEEDED(hr));

    // Write pixels into the upload resource
    void* mapped = NULL;
    D3D12_RANGE range = { 0, uploadSize };
    hr = uploadBuffer->Map(0, &range, &mapped);
    IM_ASSERT(SUCCEEDED(hr));
    for (int y = 0; y < image_height; y++)
        memcpy((void*)((uintptr_t)mapped + y * uploadPitch), image_data + y * image_width * 4, image_width * 4);
    uploadBuffer->Unmap(0, &range);

    // Copy the upload resource content into the real resource
    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = uploadBuffer;
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srcLocation.PlacedFootprint.Footprint.Width = image_width;
    srcLocation.PlacedFootprint.Footprint.Height = image_height;
    srcLocation.PlacedFootprint.Footprint.Depth = 1;
    srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

    D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
    dstLocation.pResource = pTexture;
    dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLocation.SubresourceIndex = 0;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = pTexture;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

    // Create a temporary command queue to do the copy with
    ID3D12Fence* fence = NULL;
    hr = d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    IM_ASSERT(SUCCEEDED(hr));

    HANDLE event = CreateEvent(0, 0, 0, 0);
    IM_ASSERT(event != NULL);

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 1;

    ID3D12CommandQueue* cmdQueue = NULL;
    hr = d3d_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
    IM_ASSERT(SUCCEEDED(hr));

    ID3D12CommandAllocator* cmdAlloc = NULL;
    hr = d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
    IM_ASSERT(SUCCEEDED(hr));

    ID3D12GraphicsCommandList* cmdList = NULL;
    hr = d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
    IM_ASSERT(SUCCEEDED(hr));

    cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
    cmdList->ResourceBarrier(1, &barrier);

    hr = cmdList->Close();
    IM_ASSERT(SUCCEEDED(hr));

    // Execute the copy
    cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
    hr = cmdQueue->Signal(fence, 1);
    IM_ASSERT(SUCCEEDED(hr));

    // Wait for everything to complete
    fence->SetEventOnCompletion(1, event);
    WaitForSingleObject(event, INFINITE);

    // Tear down our temporary command queue and release the upload resource
    cmdList->Release();
    cmdAlloc->Release();
    cmdQueue->Release();
    CloseHandle(event);
    fence->Release();
    uploadBuffer->Release();

    // Create a shader resource view for the texture
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    d3d_device->CreateShaderResourceView(pTexture, &srvDesc, srv_cpu_handle);

    // Return results
    *out_tex_resource = pTexture;
    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}

// Helper function to convert std::string to std::wstring
std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();
    const int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

// Helper function to get the DPI scale of a window
float GetDpiScale(HWND hWnd)
{
	const UINT dpi = GetDpiForWindow(hWnd);
	return (float)dpi / 96.0f;
}

#endif