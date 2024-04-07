#pragma once

#include <algorithm>
#include <stdexcept>

#include "imgui.h"

namespace imgui_kit
{
	namespace win32_directx12
	{
		struct WindowParameters
		{
			std::wstring title;
			int width;
			int height;

			WindowParameters(std::wstring windowTitle = L"default title", int width = 1280, int height = 720);
		};

		struct FontParameters
		{
			std::string path;
			int size;

			FontParameters(std::string path, int size = 12);
		};

		constexpr ImVec4 defaultThemeColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // A darkish gray

		struct StyleParameters
		{
			ImVec4 windowBgColor;
			ImVec2 windowPadding;
			float windowRounding;
			float transparency;
			ImVec4 textColor;
			ImVec4 themeColor;

			StyleParameters();
			StyleParameters(ImVec4 themeColor);

			void apply() const;
		};

		struct IconParameters
		{
			std::string path;
			int width;
			int height;

			IconParameters(std::string path = "", int width = 0, int height = 0);
		};

		struct BackgroundImageParameters
		{
			std::string path;
			double scale;
			int width;
			int height;

			BackgroundImageParameters(std::string path = "", double scale = 1.0);
		};

		struct UserInterfaceParameters
		{
			WindowParameters windowParameters;
			FontParameters fontParameters;
			StyleParameters styleParameters;
			IconParameters iconParameters;
			BackgroundImageParameters backgroundImageParameters;

			UserInterfaceParameters(WindowParameters windowParameters, 
				FontParameters fontParameters,
				const StyleParameters& styleParameters = {},
				IconParameters iconParameters = {},
				BackgroundImageParameters bgImgParameters ={}) ;
		};
	}
}