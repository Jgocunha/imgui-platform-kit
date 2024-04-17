
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
		std::cout << "Hello, World!" << std::endl;
		std::cout << "Current working directory: "
			<< std::filesystem::current_path()
			<< std::endl;

		using namespace imgui_kit;
		using namespace imgui_kit::glfw_opengl;

		// You can use the following parameters to customize the user interface
		const WindowParameters windowParameters(L"Hello, World!", 1920, 1280);
		const FontParameters fontParameters("../../resources/fonts/Lexend-Light.ttf", 22);
		const StyleParameters styleParameters(ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
		const IconParameters iconParameters("../../resources/icons/icon.ico");
		const BackgroundImageParameters backgroundImageParameters("../../resources/images/logo-bg.png",
			0.7);

		const UserInterfaceParameters parameters(windowParameters, 
			fontParameters, 
			styleParameters, 
			iconParameters, 
			backgroundImageParameters);
		UserInterface userInterface(parameters);

		// Or you can use the default parameters
		//UserInterface userInterface;

		userInterface.addWindow<TemplateWindow>();
		userInterface.addWindow<LogWindow>();
		//userInterface.addWindow<SomeOtherWindow>(arg1, arg2, arg3);

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
