#pragma once

/**
 * @file user_interface_parameters.h
 * @brief Configuration structures used to set up an imgui_kit::UserInterface.
 *
 * All parameters are aggregated in imgui_kit::UserInterfaceParameters, which is
 * passed to the UserInterface constructor. Window geometry can be persisted to
 * and restored from an INI file.
 */

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <GLFW/glfw3.h>
#endif

#include "imgui.h"
#include "themes.h"

namespace imgui_kit
{
	/// Default INI filename used by WindowParameters::save() and WindowParameters::load().
	constexpr char defaultWindowParametersFilename[] = "imgui_window_parameters.ini";
	/// Default INI filename reserved for persisting theme parameters.
	constexpr char defaultThemeParametersFilename[] = "imgui_theme_parameters.ini";

	/**
	 * @brief Geometry and title of the top-level platform window.
	 *
	 * A non-positive width or height (e.g. the default @c -1) is replaced with
	 * the resolution of the active monitor.
	 */
	struct WindowParameters
	{
		std::string title; ///< Text shown in the window title bar.
		int width;         ///< Window width in pixels; non-positive values use the monitor resolution.
		int height;        ///< Window height in pixels; non-positive values use the monitor resolution.
		int startPosX;     ///< Initial horizontal position of the window, in pixels.
		int startPosY;     ///< Initial vertical position of the window, in pixels.

		/**
		 * @brief Constructs window parameters.
		 * @param title Window title.
		 * @param width Window width in pixels; non-positive values use the monitor resolution.
		 * @param height Window height in pixels; non-positive values use the monitor resolution.
		 * @param startPosX Initial horizontal window position in pixels.
		 * @param startPosY Initial vertical window position in pixels.
		 */
		WindowParameters(std::string title = "default title", int width = -1, int height = -1, int startPosX = 0, int startPosY = 0);
		/**
		 * @brief Saves the window geometry to an INI file.
		 * @param filename Path of the INI file to write.
		 */
		void save(const std::string& filename = defaultWindowParametersFilename) const;
		/**
		 * @brief Loads the window geometry from an INI file previously written by save().
		 * @param filename Path of the INI file to read.
		 */
		void load(const std::string& filename = defaultWindowParametersFilename);
	};

	/**
	 * @brief Fonts to load at startup.
	 *
	 * Each entry pairs a font file path (e.g. a .ttf) with a pixel size. The
	 * first entry becomes the default ImGui font. An empty path falls back to
	 * the ImGui default font.
	 */
	struct FontParameters
	{
		std::vector<std::pair<std::string, int>> pathsAndSizes; ///< Font file paths and their pixel sizes.

		/**
		 * @brief Constructs parameters for a single font.
		 * @param path Path to the font file; empty uses the ImGui default font.
		 * @param size Font size in pixels.
		 */
		FontParameters(std::string path = "", int size = 12);
		/**
		 * @brief Constructs parameters for multiple fonts.
		 * @param pathsAndSizes Font file paths paired with their pixel sizes; the first entry is the default font.
		 */
		FontParameters(const std::vector<std::pair<std::string, int>>& pathsAndSizes);
	};

	/**
	 * @brief Visual style of the user interface (theme and window styling).
	 *
	 * apply() selects #theme, then forwards geometry and colour fields to
	 * ImGui::GetStyle() (#windowBgColor, #windowPadding, #framePadding,
	 * #windowRounding, #transparency, #textColor).
	 */
	struct StyleParameters
	{
		Theme theme;          ///< Colour theme applied to the interface (see imgui_kit::Theme).
		ImVec4 windowBgColor; ///< Background colour of the main window.
		ImVec2 windowPadding; ///< Padding within windows, in pixels.
		ImVec2 framePadding;  ///< Padding within framed widgets (buttons, inputs), in pixels.
		float windowRounding; ///< Corner rounding radius of windows, in pixels.
		float transparency;   ///< Overall window transparency (alpha), 0.0-1.0.
		ImVec4 textColor;     ///< Default text colour.

		/// Constructs style parameters with the default theme.
		StyleParameters();
		/**
		 * @brief Constructs style parameters with the given theme.
		 * @param theme Colour theme to apply.
		 */
		StyleParameters(Theme theme);
		/**
		 * @brief Constructs style parameters with the given theme and background colour.
		 * @param theme Colour theme to apply.
		 * @param bgColor Background colour of the main window.
		 */
		StyleParameters(Theme theme, ImVec4 bgColor);

		/**
		 * @brief Applies the style to the current ImGui context.
		 * Applies #theme, then forwards geometry and colour fields (including frame padding) to ImGui::GetStyle().
		 */
		void apply() const;
	};

	/**
	 * @brief Window icon settings.
	 *
	 * On Windows an @c .ico file is expected; on Linux a @c .png file.
	 * @note On macOS setting a window icon is a no-op (GLFW limitation) â€” the
	 * application icon comes from the app bundle instead.
	 */
	struct IconParameters
	{
		std::string path; ///< Path to the icon file (.ico on Windows, .png on Linux).
		int width;        ///< Icon width in pixels (0 keeps the file's native size).
		int height;       ///< Icon height in pixels (0 keeps the file's native size).

		/**
		 * @brief Constructs icon parameters.
		 * @param path Path to the icon file; empty means no icon.
		 * @param width Icon width in pixels; 0 keeps the native size.
		 * @param height Icon height in pixels; 0 keeps the native size.
		 */
		IconParameters(std::string path = "", int width = 0, int height = 0);
	};

	/// How a background image is scaled to the window.
	enum class ImageFitType : uint8_t
	{
		KEEP_ASPECT_RATIO = 0, ///< Scale the image preserving its aspect ratio.
		ZOOM_TO_FIT,           ///< Stretch the image to fill the window.
	};

	/**
	 * @brief Background image drawn behind all windows.
	 */
	struct BackgroundImageParameters
	{
		std::string path;      ///< Path to the image file; empty means no background image.
		ImageFitType fitType;  ///< How the image is scaled to the window.
		int width;             ///< Native image width in pixels (set when the image is loaded).
		int height;            ///< Native image height in pixels (set when the image is loaded).

		/**
		 * @brief Constructs background image parameters.
		 * @param path Path to the image file; empty means no background image.
		 * @param fitType How the image is scaled to the window.
		 */
		BackgroundImageParameters(std::string path = "", ImageFitType fitType = ImageFitType::KEEP_ASPECT_RATIO);
	};

	/**
	 * @brief Aggregates all configuration passed to imgui_kit::UserInterface.
	 *
	 * Bundles window geometry, fonts, style, icon and background image
	 * settings. Every member has a sensible default, so a default-constructed
	 * instance yields a working interface.
	 */
	struct UserInterfaceParameters
	{
		WindowParameters windowParameters;                     ///< Top-level window geometry and title.
		FontParameters fontParameters;                         ///< Fonts to load at startup.
		StyleParameters styleParameters;                       ///< Theme and window styling.
		IconParameters iconParameters;                         ///< Window icon settings.
		BackgroundImageParameters backgroundImageParameters;   ///< Background image settings.

		/**
		 * @brief Constructs the aggregated parameters.
		 * @param windowParameters Top-level window geometry and title.
		 * @param fontParameters Fonts to load at startup.
		 * @param styleParameters Theme and window styling.
		 * @param iconParameters Window icon settings.
		 * @param bgImgParameters Background image settings.
		 */
		UserInterfaceParameters(WindowParameters windowParameters = {},
			FontParameters fontParameters = {},
			const StyleParameters& styleParameters = {},
			IconParameters iconParameters = {},
			BackgroundImageParameters bgImgParameters ={}) ;
		/**
		 * @brief Saves the window geometry to an INI file.
		 * @param filename Path of the INI file to write.
		 */
		void save(const std::string& filename = defaultWindowParametersFilename) const;
	};
}
