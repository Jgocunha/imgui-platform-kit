# Configuration

All configuration is passed through `UserInterfaceParameters`, which groups five sub-parameter structs. Every field has a default value, so you can start with `UserInterface{}` and only set what you need.

```cpp
#include "user_interface.h"
using namespace imgui_kit;

UserInterfaceParameters params(
    windowParameters,
    fontParameters,
    styleParameters,
    iconParameters,
    backgroundImageParameters
);
UserInterface ui(params);
```

---

## WindowParameters

Controls the OS window.

```cpp
WindowParameters(
    std::string title   = "default title",
    int width           = -1,   // -1 = use primary monitor width
    int height          = -1,   // -1 = use primary monitor height
    int startPosX       = 0,
    int startPosY       = 0
);
```

Parameters are saved to and loaded from `imgui_window_parameters.ini` automatically. Pass explicit positive dimensions to avoid the monitor-query path (required on headless systems).

---

## FontParameters

Loads one or more fonts from `.ttf` / `.otf` files.

```cpp
// Single font
FontParameters(std::string path, int size = 12);

// Multiple fonts — first entry is the default
FontParameters(const std::vector<std::pair<std::string, int>>& pathsAndSizes);
```

**Example:**
```cpp
FontParameters fonts({
    { PROJECT_DIR + "/resources/fonts/JetBrainsMono-Regular.ttf", 20 },
    { PROJECT_DIR + "/resources/fonts/JetBrainsMono-Bold.ttf",    20 },
});
```

---

## StyleParameters

Sets the visual theme and window styling.

```cpp
StyleParameters();                              // defaults to Dark theme
StyleParameters(Theme theme);
StyleParameters(Theme theme, ImVec4 bgColor);
```

| Field | Type | Description |
|---|---|---|
| `theme` | `Theme` | One of the 39 available themes (see [Themes and Colours](Themes-and-Colours)) |
| `windowBgColor` | `ImVec4` | Background colour of ImGui windows |
| `windowPadding` | `ImVec2` | Inner padding |
| `windowRounding` | `float` | Corner rounding radius |
| `transparency` | `float` | Window transparency (`1.0` = fully opaque, `0.0` = fully transparent) |
| `textColor` | `ImVec4` | Default text colour |

> **Note:** `apply()` currently only applies the selected `theme`. The fields `windowBgColor`, `windowPadding`, `windowRounding`, `transparency`, and `textColor` are stored but not yet forwarded to `ImGui::GetStyle()`. To apply them yourself, call `ImGui::GetStyle()` directly after `userInterface.initialize()`.

---

## IconParameters

Sets the window/taskbar icon.

```cpp
IconParameters(std::string path = "", int width = 0, int height = 0);
```

- **Windows**: use `.ico` files
- **Linux**: use `.png` files
- **macOS**: `glfwSetWindowIcon` is silently ignored — the icon has no visual effect

```cpp
#ifdef _WIN32
IconParameters icon(PROJECT_DIR + "/resources/icons/icon.ico");
#else
IconParameters icon(PROJECT_DIR + "/resources/icons/icon.png");
#endif
```

---

## BackgroundImageParameters

Renders a full-window background image.

```cpp
BackgroundImageParameters(
    std::string path    = "",
    ImageFitType fitType = ImageFitType::KEEP_ASPECT_RATIO
);
```

| `ImageFitType` | Behaviour |
|---|---|
| `KEEP_ASPECT_RATIO` | Fit inside the window, preserving the image ratio |
| `ZOOM_TO_FIT` | Stretch to fill the entire window |

---

## Persistence

`WindowParameters` saves/loads from `imgui_window_parameters.ini` in the working directory. Call `save()` and `load()` explicitly, or let the constructor handle it:

```cpp
params.save("my_config.ini");
```
