
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <exception>
#include <filesystem>
#include <iostream>

#include "user_interface.h"
#include "template_window.h"
#include "log_window.h"

int main(int argc, char* argv[])
{
	try
	{
		using namespace imgui_kit;

		// You can use the following parameters to customize the user interface
		const WindowParameters windowParameters("imgui-platform-kit!");
		const FontParameters fontParameters({	{std::string(PROJECT_DIR) + "/resources/fonts/JetBrainsMono-Regular.ttf", 20},
												{std::string(PROJECT_DIR) + "/resources/fonts/JetBrainsMono-Thin.ttf", 20},
												{std::string(PROJECT_DIR) + "/resources/fonts/JetBrainsMono-Medium.ttf", 20},
												{std::string(PROJECT_DIR) + "/resources/fonts/JetBrainsMono-Bold.ttf", 20},
												{std::string(PROJECT_DIR) + "/resources/fonts/JetBrainsMono-Italic.ttf", 20},
												{std::string(PROJECT_DIR) + "/resources/fonts/JetBrainsMono-Light.ttf", 20},
		});
		constexpr ImVec4 backgroundColor = colours::DarkGray;
		const StyleParameters styleParameters(Theme::Windark, backgroundColor);
		#ifdef _WIN32
		const IconParameters iconParameters(std::string(PROJECT_DIR) + "/resources/icons/icon.ico");
		#elif defined(__linux__) || defined(__APPLE__)
		const IconParameters iconParameters(std::string(PROJECT_DIR) + "/resources/icons/icon.png");
		#endif
		const BackgroundImageParameters backgroundImageParameters(std::string(PROJECT_DIR) + "/resources/images/logo-bg.png");

		const UserInterfaceParameters parameters(windowParameters, 
			fontParameters, 
			styleParameters, 
			iconParameters, 
			backgroundImageParameters);
		UserInterface userInterface(parameters);

		// Or you can use the default parameters
		//UserInterface userInterface;

		// In any part of your code, you can modify the global flags:
		//imgui_kit::setGlobalWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		// Or add specific flags:
		imgui_kit::addGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);
		// Or remove specific flags:
		//imgui_kit::removeGlobalWindowFlags(ImGuiWindowFlags_NoMove);

		userInterface.addWindow<TemplateWindow>();
		userInterface.addWindow<LogWindow>();
		//userInterface.addWindow<SomeOtherWindow>(arg1, arg2, arg3);

		LogWindow::addLog(colours::Green, "[DEBUG]   Debug message.");
		LogWindow::addLog(colours::White, "[INFO]    Informative message.");
		LogWindow::addLog(colours::Yellow,"[WARNING] Warning message.");
		LogWindow::addLog(colours::Red,   "[ERROR]   Error message.");
		LogWindow::addLog(colours::Red,   "[FATAL]   Fatal error message.");

		userInterface.initialize();
		while(!userInterface.isShutdownRequested())
			userInterface.render();
		userInterface.shutdown();

		return 0;
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
}