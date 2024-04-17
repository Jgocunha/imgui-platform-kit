#include "user_interface.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace imgui_kit
{
	namespace glfw_opengl
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
                return;

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

            // Create window with graphics context
            window = glfwCreateWindow(parameters.windowParameters.width, 
            parameters.windowParameters.height, 
            parameters.windowParameters.title.c_str(), nullptr, nullptr);
            if (window == nullptr)
                return;
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSwapInterval(1); // Enable vsync

            loadIcon();

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

            // Setup Dear ImGui style
            parameters.styleParameters.apply();
            

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
        #ifdef __EMSCRIPTEN__
            ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
        #endif
            ImGui_ImplOpenGL3_Init(glsl_version);

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

            ImGuiIO& io = ImGui::GetIO(); (void)io;
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
		}

		void UserInterface::shutdown()
		{
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
            const ImGuiIO& io_ref = ImGui::GetIO(); (void)io_ref;

            if (!std::filesystem::exists(parameters.fontParameters.path))
            {
                std::cerr << "Font file does not exist: " << parameters.fontParameters.path << std::endl;
                io_ref.Fonts->AddFontDefault();
                return;
            }

            const ImFont* font = io_ref.Fonts->AddFontFromFileTTF(parameters.fontParameters.path.c_str(), static_cast<float>(parameters.fontParameters.size));
            if (font == nullptr)
            {
                io_ref.Fonts->AddFontDefault();
				std::cerr << "Failed to load font." << std::endl;
            }
		}

        void UserInterface::loadBackgroundImage()
        {
            bool ret = LoadTextureFromFile(backgroundImageTexture.parameters.path.c_str(), 
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
            // Obtain the main viewport
            ImGuiViewport* viewport = ImGui::GetMainViewport();

            // Use the viewport's size to determine how to center the image
            const ImVec2 windowSize = viewport->Size; // This is the size of the area we can draw in

            // Calculate the image's dimensions
            static const ImVec2 imageSize = ImVec2((float)backgroundImageTexture.parameters.width * (float)backgroundImageTexture.parameters.scale,
                (float)backgroundImageTexture.parameters.height * (float)backgroundImageTexture.parameters.scale);

            // Calculate the top-left position to center the image in the viewport
            const ImVec2 pos = ImVec2(viewport->Pos.x + (windowSize.x - imageSize.x) * 0.5f,
                viewport->Pos.y + (windowSize.y - imageSize.y) * 0.5f);

            // Get the viewport's background draw list and add the image
            ImGui::GetBackgroundDrawList(viewport)->AddImage(
                (ImTextureID)backgroundImageTexture.texture,
                pos, ImVec2(pos.x + imageSize.x, pos.y + imageSize.y));
        }
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

