# Themes and Colours

## Applying a theme

Pass a `Theme` value inside `StyleParameters`:

```cpp
using namespace imgui_kit;

StyleParameters style(Theme::Windark);
// or with a custom background colour:
StyleParameters style(Theme::Windark, colours::DarkGray);
```

## Available themes

39 themes are available from the `Theme` enum (sourced from [ImThemes](https://github.com/Patitotective/ImThemes)):

| Name | Enum value |
|---|---|
| Light | `Theme::Light` |
| Dark | `Theme::Dark` |
| Classic | `Theme::Classic` |
| Adobe Inspired | `Theme::AdobeInspired` |
| Black Devil | `Theme::BlackDevil` |
| Bootstrap Dark | `Theme::BootstrapDark` |
| Cherry | `Theme::Cherry` |
| Classic Steam | `Theme::ClassicSteam` |
| Clean Dark Red | `Theme::CleanDarkRed` |
| Comfy | `Theme::Comfy` |
| Duck Red | `Theme::DuckRed` |
| Darcula | `Theme::Darcula` |
| Dark Ruda | `Theme::DarkRuda` |
| Darky | `Theme::Darky` |
| Deep Dark | `Theme::DeepDark` |
| Discord Dark | `Theme::DiscordDark` |
| Enemy Mouse | `Theme::EnemyMouse` |
| Ever Forest | `Theme::EverForest` |
| Future Dark | `Theme::FutureDark` |
| Gold | `Theme::Gold` |
| Green Font | `Theme::GreenFont` |
| Green Leaf | `Theme::GreenLeaf` |
| Hazy Dark | `Theme::HazyDark` |
| Material Flat | `Theme::MaterialFlat` |
| Microsoft | `Theme::Microsoft` |
| Modern | `Theme::Modern` |
| Moonlight | `Theme::Moonlight` |
| Photoshoop | `Theme::Photoshoop` |
| Purple Comfy | `Theme::PurpleComfy` |
| Quick Minimal | `Theme::QuickMinimal` |
| Red Font | `Theme::RedFont` |
| Red Oni | `Theme::RedOni` |
| Rounded Visual Studio | `Theme::RoundedVisualStudio` |
| Soft Cherry | `Theme::SoftCherry` |
| Sonic Riders | `Theme::SonicRiders` |
| Unreal | `Theme::Unreal` |
| Visual Studio | `Theme::VisualStudio` |
| Windark | `Theme::Windark` |
| Led Synthmaster | `Theme::LedSynthmaster` |

## Runtime theme selector

Show a built-in theme-picker popup in any window:

```cpp
bool open = true;
imgui_kit::showImGuiKitThemeSelector(&open);
```

## ColourPalette

`colour_palette.h` provides named `ImVec4` constants in the `imgui_kit::colours` namespace, grouped by category:

**Basic:**
`White`, `Black`, `Red`, `Green`, `Blue`, `Yellow`, `Orange`, `Purple`, `Cyan`, `Magenta`

**Extended:**
`Coral`, `Violet`, `Gold`, `Azure`, `Beige`, `Lime`, `Teal`, `Olive`, `Maroon`, `Navy`, `Mint`, `Peach`

**Grayscale:**
`DarkGray`, `Gray`, `LightGray`, `Silver`

**Pastel:**
`PastelPink`, `PastelGreen`, `PastelBlue`, `PastelYellow`, `PastelOrange`, `PastelViolet`

**Dark:**
`DarkRed`, `DarkGreen`, `DarkBlue`, `DarkYellow`, `DarkPurple`, `DarkCyan`, `DarkMagenta`, `DarkOrange`

**Usage:**

```cpp
#include "colour_palette.h"
using namespace imgui_kit;

ImGui::TextColored(colours::Coral, "Warning text");
LogWindow::addLog(colours::Yellow, "[WARNING] Something happened.");
StyleParameters style(Theme::Dark, colours::DarkGray);
```
