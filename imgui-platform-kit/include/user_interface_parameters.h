#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <GLFW/glfw3.h>
#endif

#include "imgui.h"
#include "themes.h"

namespace imgui_kit
{
	constexpr char defaultWindowParametersFilename[] = "imgui_window_parameters.ini";
	constexpr char defaultThemeParametersFilename[] = "imgui_theme_parameters.ini";

	struct WindowParameters
	{
		std::string title;
		int width;
		int height;
		int startPosX;
		int startPosY;

		WindowParameters(std::string title = "default title", int width = -1, int height = -1, int startPosX = 0, int startPosY = 0);
		void save(const std::string& filename = defaultWindowParametersFilename) const;
		void load(const std::string& filename = defaultWindowParametersFilename);
	};

	struct FontParameters
	{
		std::vector<std::pair<std::string, int>> pathsAndSizes;

		FontParameters(std::string path = "", int size = 12);
		FontParameters(const std::vector<std::pair<std::string, int>>& pathsAndSizes);
	};

	struct StyleParameters
	{
		Theme theme;
		ImVec4 windowBgColor;
		ImVec2 windowPadding;
		float windowRounding;
		float transparency;
		ImVec4 textColor;

		StyleParameters();
		StyleParameters(Theme theme);
		StyleParameters(Theme theme, ImVec4 bgColor);

		void apply() const;
	};

	struct IconParameters
	{
		std::string path;
		int width;
		int height;

		IconParameters(std::string path = "", int width = 0, int height = 0);
	};

	enum class ImageFitType : uint8_t
	{
		KEEP_ASPECT_RATIO = 0,
		ZOOM_TO_FIT,
	};

	struct BackgroundImageParameters
	{
		std::string path;
		ImageFitType fitType;
		int width;
		int height;

		BackgroundImageParameters(std::string path = "", ImageFitType fitType = ImageFitType::KEEP_ASPECT_RATIO);
	};

	struct UserInterfaceParameters
	{
		WindowParameters windowParameters;
		FontParameters fontParameters;
		StyleParameters styleParameters;
		IconParameters iconParameters;
		BackgroundImageParameters backgroundImageParameters;

		UserInterfaceParameters(WindowParameters windowParameters = {},
			FontParameters fontParameters = {},
			const StyleParameters& styleParameters = {},
			IconParameters iconParameters = {},
			BackgroundImageParameters bgImgParameters ={}) ;
		void save(const std::string& filename = defaultWindowParametersFilename) const;
	};
}