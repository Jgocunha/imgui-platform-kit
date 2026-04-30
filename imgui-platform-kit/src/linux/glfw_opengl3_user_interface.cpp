
#if defined(__linux__)

#include "user_interface.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace imgui_kit
{

    UserInterface::UserInterface()
        : parameters()
        , backgroundImageTexture()
        , window(nullptr)
        , shutdownRequest(false)
    {}

    UserInterface::UserInterface(UserInterfaceParameters parameters)
        : parameters(std::move(parameters))
        , backgroundImageTexture(this->parameters.backgroundImageParameters)
        , window(nullptr)
        , shutdownRequest(false)
    {}

    void UserInterface::initialize()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW." << std::endl;
            return;
        }

            // Decide GL+GLSL versions
        #if defined(IMGUI_IMPL_OPENGL_ES2)
            // GL ES 2.0 + GLSL 100
            const char* glsl_version = "#version 100";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        #elif defined(__APPLE__)
            // GL 3.2 + GLSL 150
            const char* glsl_version = "#version 150";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
        #else
            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
        #endif

        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

        // Create window with graphics context
        window = glfwCreateWindow(parameters.windowParameters.width,
        parameters.windowParameters.height,
        parameters.windowParameters.title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            glfwTerminate();
            return;
        }
        glfwSetWindowPos(window, parameters.windowParameters.startPosX,
            parameters.windowParameters.startPosY);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui and ImPlot context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
    #ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
    #endif
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup Dear ImGui style
        parameters.styleParameters.apply();
        loadIcon();
        loadFont();
        loadBackgroundImage();
    }

    void UserInterface::render()
    {
        shutdownRequest = glfwWindowShouldClose(window);

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderBackgroundImage();
        renderWindows();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(parameters.styleParameters.windowBgColor.x * parameters.styleParameters.windowBgColor.w,
            parameters.styleParameters.windowBgColor.y * parameters.styleParameters.windowBgColor.w,
            parameters.styleParameters.windowBgColor.z * parameters.styleParameters.windowBgColor.w,
            parameters.styleParameters.windowBgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        const ImGuiIO& io = ImGui::GetIO(); (void)io;
        updateFontGlobalScale();
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);

        updateLastRenderedFrameDimensions();
    }

    void UserInterface::shutdown() const
    {
        parameters.save();
        ImNodeEditor::DestroyEditor(ImNodeEditor::GetCurrentEditor());
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool UserInterface::isShutdownRequested() const
    {
        return shutdownRequest;
    }

    void UserInterface::loadIcon() const
    {
        int width, height, channels;
        unsigned char* data = stbi_load(parameters.iconParameters.path.c_str(), 
        &width, &height, &channels, 4); // Load with 4 channels (RGBA)

        if (data) {
            GLFWimage image;
            image.width = width;
            image.height = height;
            image.pixels = data;

            glfwSetWindowIcon(window, 1, &image);
            stbi_image_free(data); // Free image data memory after setting the icon
        } else {
            std::cerr << "Failed to load icon. Error: " << stbi_failure_reason() << std::endl;
        }
    }

    void UserInterface::loadFont()
    {
        ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;

        bool fontLoaded = false;

        for (const auto& [path, size] : parameters.fontParameters.pathsAndSizes)
        {
            // Check if the font file exists
            if (!std::filesystem::exists(path))
            {
                std::cerr << "Font file does not exist: " << path << std::endl;
                continue; // Try the next font in the list
            }

            // Attempt to load the font
            const ImFont* font = io_ref.Fonts->AddFontFromFileTTF(path.c_str(), static_cast<float>(size));
            if (font == nullptr)
            {
                std::cerr << "Failed to load font: " << path << " with size " << size << std::endl;
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
        io_ref.FontGlobalScale = GetDpiScale(window);
    }


    void UserInterface::loadBackgroundImage()
    {
        const bool ret = LoadTextureFromFile(backgroundImageTexture.parameters.path.c_str(),
        &backgroundImageTexture.texture, 
        &backgroundImageTexture.parameters.width, 
        &backgroundImageTexture.parameters.height);
        if (!ret)
        {
            std::cerr << "Failed to load background image at : " << backgroundImageTexture.parameters.path <<
            " Error: " << stbi_failure_reason() << std::endl;
            return;
        }
    }

    void UserInterface::renderWindows() const
    {
        for (const auto& window : windows)
            window->render();
    }

    void UserInterface::renderBackgroundImage() const
    {
        if (backgroundImageTexture.texture == 0 ||
            backgroundImageTexture.parameters.width <= 0 ||
            backgroundImageTexture.parameters.height <= 0)
            return;

        // Obtain the main viewport
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Use the viewport's size to determine how to center the image
        const ImVec2 windowSize = viewport->Size; // This is the size of the area we can draw in

        const float dpiScale = GetDpiScale(window);

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
            (ImTextureID)backgroundImageTexture.texture,
            pos, ImVec2(pos.x + imageSize.x, pos.y + imageSize.y));
    }	

    void UserInterface::updateLastRenderedFrameDimensions()
    {
        if (window != nullptr)
        {
            int width, height, posX, posY;
            glfwGetWindowSize(window, &width, &height);
            glfwGetWindowPos(window, &posX, &posY);
            parameters.windowParameters.width = width;
            parameters.windowParameters.height = height;
            parameters.windowParameters.startPosX = posX;
            parameters.windowParameters.startPosY = posY;
        }
        else
            std::cerr << "Invalid GLFW window." << std::endl;
    }

    void UserInterface::updateFontGlobalScale() const
    {
        ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;
        io_ref.FontGlobalScale = GetDpiScale(window);
    }
}


// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Helper function to find the monitor where the window is currently located
GLFWmonitor* GetActiveMonitor(GLFWwindow* window) 
{
    int windowX, windowY, windowWidth, windowHeight;
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    glfwGetWindowPos(window, &windowX, &windowY);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    for (int i = 0; i < monitorCount; i++) {
        int monitorX, monitorY;
        const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &monitorX, &monitorY);

        if (windowX >= monitorX &&
            windowY >= monitorY &&
            windowX + windowWidth <= monitorX + mode->width &&
            windowY + windowHeight <= monitorY + mode->height) 
        {
            return monitors[i];
        }
    }
    // Return the primary monitor if the window is not fully on any monitor
    return glfwGetPrimaryMonitor();
}

// Helper function to get the DPI scale of a window
float GetDpiScale(GLFWwindow* window)
{
	float xscale, yscale;
    GLFWmonitor* monitor = GetActiveMonitor(window);
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    return yscale;
}

#endif