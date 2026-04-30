#include "user_interface_parameters.h"
#include "user_interface_parameters.h"

#include "colour_palette.h"

namespace imgui_kit
{
	WindowParameters::WindowParameters(std::string title, int width, int height, int startPosX, int startPosY)
		: title(std::move(title)), width(width), height(height), startPosX(startPosX), startPosY(startPosY)
	{
		load();
#if defined(_WIN32)
		if (this->width <= 0 || this->height <= 0)
		{
			const HMONITOR hMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(hMonitor, &mi))
			{
				this->width = mi.rcMonitor.right - mi.rcMonitor.left;
				this->height = mi.rcMonitor.bottom - mi.rcMonitor.top;
			}
			else
			{
				RECT desktop;
				const HWND hDesktop = GetDesktopWindow();
				GetWindowRect(hDesktop, &desktop);
				this->width = desktop.right;
				this->height = desktop.bottom;
			}
		}
#elif defined(__linux__) || defined(__APPLE__)
		if (this->width <= 0 || this->height <= 0)
		{
			if (!glfwInit())
				throw std::runtime_error("Failed to initialize GLFW.");

			// Get the primary monitor
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			if (primaryMonitor == nullptr)
				throw std::runtime_error("Failed to get the primary monitor.");

			// Get the current video mode of the monitor
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
			if (mode == nullptr)
				throw std::runtime_error("Failed to get the video mode of the primary monitor.");

			// Set width and height to the monitor's resolution if they were invalid
			this->width = mode->width;
			this->height = mode->height;

			glfwTerminate();
		}
#endif

		if (this->width <= 0)  
			throw std::invalid_argument("Width must be greater than 0.");
		if (this->height <= 0)  
			throw std::invalid_argument("Height must be greater than 0.");
	}

	void WindowParameters::save(const std::string& filename) const
	{
		std::ofstream file(filename);
		if (!file.is_open())
			throw std::runtime_error("Could not open file for writing.");

		file << "[Window Parameters]\n";
		file << "Title=" << title << "\n";
		file << "Width=" << width << "\n";
		file << "Height=" << height << "\n";
		file << "StartPosX=" << startPosX << "\n";
		file << "StartPosY=" << startPosY << "\n";

		file.close();
		if (file.fail())
			throw std::runtime_error("Failed to save window parameters.");
	}

	void WindowParameters::load(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file.is_open())
			return; // If the file does not exist, skip loading

		std::string line;
		while (std::getline(file, line))
		{
			if (line.find("Title=") == 0)
				title = line.substr(6);
			else if (line.find("Width=") == 0)
				width = std::stoi(line.substr(6));
			else if (line.find("Height=") == 0)
				height = std::stoi(line.substr(7));
			else if (line.find("StartPosX=") == 0)
				startPosX = std::stoi(line.substr(10));
			else if (line.find("StartPosY=") == 0)
				startPosY = std::stoi(line.substr(10));
		}

		file.close();
	}

	FontParameters::FontParameters(std::string path, int size)
	{
		pathsAndSizes = { {std::move(path), size} };
		if (size <= 0)
			throw std::invalid_argument("Size must be greater than 0.");
	}

	FontParameters::FontParameters(const std::vector<std::pair<std::string, int>>& pathsAndSizes)
		: pathsAndSizes(pathsAndSizes)
	{
		for (const auto& pathAndSize : pathsAndSizes)
		{
			const auto& [path, size] = pathAndSize;
			if (size <= 0)
			{
				throw std::invalid_argument("Font size must be greater than 0.");
			}
			if (path.empty())
			{
				throw std::invalid_argument("Font path cannot be empty.");
			}
		}
	}

	StyleParameters::StyleParameters()
		: theme(Theme::Dark)
	{
		windowBgColor = colours::DarkGray;
		windowRounding = 5.0f;
		windowPadding = ImVec2(8.0f, 8.0f);
		transparency = 1.0f;
		textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	StyleParameters::StyleParameters(Theme theme)
		: theme(theme)
	{
		windowBgColor = colours::DarkGray;
		windowRounding = 5.0f;
		windowPadding = ImVec2(8.0f, 8.0f);
		transparency = 1.0f;
		textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	StyleParameters::StyleParameters(Theme theme, ImVec4 bgColor)
		: theme(theme), windowBgColor(bgColor)
	{
		windowRounding = 5.0f;
		windowPadding = ImVec2(8.0f, 8.0f);
		transparency = 1.0f;
		textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void StyleParameters::apply() const
	{
		themes[theme]();
	}

	IconParameters::IconParameters(std::string path, int width, int height)
		: path(std::move(path)), width(width), height(height)
	{}


	BackgroundImageParameters::BackgroundImageParameters(std::string path, ImageFitType fitType)
		: path(std::move(path)), fitType(fitType), width(0), height(0)
	{}

	UserInterfaceParameters::UserInterfaceParameters(WindowParameters windowParameters,
		FontParameters fontParameters,
		const StyleParameters& styleParameters,
		IconParameters iconParameters,
		BackgroundImageParameters bgImgParameters)
			: windowParameters(std::move(windowParameters)),
				fontParameters(std::move(fontParameters)),
				styleParameters(styleParameters),
				iconParameters(std::move(iconParameters)),
				backgroundImageParameters(std::move(bgImgParameters))
	{}

	void UserInterfaceParameters::save(const std::string& filename) const
	{
		windowParameters.save(filename);
	}
}


  