#pragma once

#include "imgui.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace imgui_kit
{
    void showImGuiKitThemeSelector(bool* p_open = nullptr);

    // theme list from https://github.com/Patitotective/ImThemes
    enum class Theme
    {
        Light,
        Dark,
        Classic,
        AdobeInspired,
        BlackDevil,
        BootstrapDark,
        Cherry,
        ClassicSteam,
        CleanDarkRed,
        Comfy,
        DuckRed,
        Darcula,
        DarkRuda,
        Darky,
        DeepDark,
        DiscordDark,
        EnemyMouse,
        EverForest,
        FutureDark,
        Gold,
        GreenFont,
        GreenLeaf,
        HazyDark,
        MaterialFlat,
        Microsoft,
        Modern,
        Moonlight,
        Photoshoop,
        PurpleComfy,
        QuickMinimal,
        RedFont,
        RedOni,
        RoundedVisualStudio,
        SoftCherry,
        SonicRiders,
        Unreal,
        VisualStudio,
        Windark,
        LedSynthmaster
	};

    inline const std::unordered_map<Theme, std::string> themeNames = {
	    {Theme::Light, "Light"},
	    {Theme::Dark, "Dark"},
	    {Theme::Classic, "Classic"},
	    {Theme::AdobeInspired, "Adobe Inspired"},
	    {Theme::BlackDevil, "Black Devil"},
	    {Theme::BootstrapDark, "Bootstrap Dark"},
	    {Theme::Cherry, "Cherry"},
	    {Theme::ClassicSteam, "Classic Steam"},
	    {Theme::CleanDarkRed, "Clean Dark Red"},
	    {Theme::Comfy, "Comfy"},
	    {Theme::DuckRed, "Duck Red"},
	    {Theme::Darcula, "Darcula"},
	    {Theme::DarkRuda, "Dark Ruda"},
	    {Theme::Darky, "Darky"},
	    {Theme::DeepDark, "Deep Dark"},
	    {Theme::DiscordDark, "Discord Dark"},
	    {Theme::EnemyMouse, "Enemy Mouse"},
	    {Theme::EverForest, "Ever Forest"},
	    {Theme::FutureDark, "Future Dark"},
	    {Theme::Gold, "Gold"},
	    {Theme::GreenFont, "Green Font"},
	    {Theme::GreenLeaf, "Green Leaf"},
	    {Theme::HazyDark, "Hazy Dark"},
	    {Theme::MaterialFlat, "Material Flat"},
	    {Theme::Microsoft, "Microsoft"},
	    {Theme::Modern, "Modern"},
	    {Theme::Moonlight, "Moonlight"},
	    {Theme::Photoshoop, "Photoshoop"},
	    {Theme::PurpleComfy, "Purple Comfy"},
	    {Theme::QuickMinimal, "Quick Minimal"},
	    {Theme::RedFont, "Red Font"},
	    {Theme::RedOni, "Red Oni"},
	    {Theme::RoundedVisualStudio, "Rounded Visual Studio"},
	    {Theme::SoftCherry, "Soft Cherry"},
	    {Theme::SonicRiders, "Sonic Riders"},
	    {Theme::Unreal, "Unreal"},
	    {Theme::VisualStudio, "Visual Studio"},
	    {Theme::Windark, "Windark"},
	    {Theme::LedSynthmaster, "Led Synthmaster"}
    };

    // Stub functions for theme application
    void applyAdobeInspiredTheme();
    void applyBlackDevilTheme();
    void applyBootstrapDarkTheme();
    void applyCherryTheme();
    void applyClassicSteamTheme();
    void applyCleanDarkRedTheme();
    void applyComfyTheme();
    void applyDuckRedTheme();
    void applyDarculaTheme();
    void applyDarkRudaTheme();
    void applyDarkyTheme();
    void applyDeepDarkTheme();
    void applyDiscordDarkTheme();
    void applyEnemyMouseTheme();
    void applyEverForestTheme();
    void applyFutureDarkTheme();
    void applyGoldTheme();
    void applyGreenFontTheme();
    void applyGreenLeafTheme();
    void applyHazyDarkTheme();
    void applyMaterialFlatTheme();
    void applyMicrosoftTheme();
    void applyModernTheme();
    void applyMoonlightTheme();
    void applyPhotoshoopTheme();
    void applyPurpleComfyTheme();
    void applyQuickMinimalTheme();
    void applyRedFontTheme();
    void applyRedOniTheme();
    void applyRoundedVisualStudioTheme();
    void applySoftCherryTheme();
    void applySonicRidersTheme();
    void applyUnrealTheme();
    void applyVisualStudioTheme();
    void applyWindarkTheme();
    void applyLedSynthmasterTheme();

    inline std::unordered_map<Theme, std::function<void()>> themes = {
        {Theme::Light, []() { ImGui::StyleColorsLight(); }},
        {Theme::Dark, []() { ImGui::StyleColorsDark(); }},
        {Theme::Classic, []() { ImGui::StyleColorsClassic(); }},
        {Theme::AdobeInspired, []() { applyAdobeInspiredTheme(); }},
        {Theme::BlackDevil, []() { applyBlackDevilTheme(); }},
        {Theme::BootstrapDark, []() { applyBootstrapDarkTheme(); }},
        {Theme::Cherry, []() { applyCherryTheme(); }},
        {Theme::ClassicSteam, []() { applyClassicSteamTheme(); }},
        {Theme::CleanDarkRed, []() { applyCleanDarkRedTheme(); }},
        {Theme::Comfy, []() { applyComfyTheme(); }},
        {Theme::DuckRed, []() { applyDuckRedTheme(); }},
        {Theme::Darcula, []() { applyDarculaTheme(); }},
        {Theme::DarkRuda, []() { applyDarkRudaTheme(); }},
        {Theme::Darky, []() { applyDarkyTheme(); }},
        {Theme::DeepDark, []() { applyDeepDarkTheme(); }},
        {Theme::DiscordDark, []() { applyDiscordDarkTheme(); }},
        {Theme::EnemyMouse, []() { applyEnemyMouseTheme(); }},
        {Theme::EverForest, []() { applyEverForestTheme(); }},
        {Theme::FutureDark, []() { applyFutureDarkTheme(); }},
        {Theme::Gold, []() { applyGoldTheme(); }},
        {Theme::GreenFont, []() { applyGreenFontTheme(); }},
        {Theme::GreenLeaf, []() { applyGreenLeafTheme(); }},
        {Theme::HazyDark, []() { applyHazyDarkTheme(); }},
        {Theme::MaterialFlat, []() { applyMaterialFlatTheme(); }},
        {Theme::Microsoft, []() { applyMicrosoftTheme(); }},
        {Theme::Modern, []() { applyModernTheme(); }},
        {Theme::Moonlight, []() { applyMoonlightTheme(); }},
        {Theme::Photoshoop, []() { applyPhotoshoopTheme(); }},
        {Theme::PurpleComfy, []() { applyPurpleComfyTheme(); }},
        {Theme::QuickMinimal, []() { applyQuickMinimalTheme(); }},
        {Theme::RedFont, []() { applyRedFontTheme(); }},
        {Theme::RedOni, []() { applyRedOniTheme(); }},
        {Theme::RoundedVisualStudio, []() { applyRoundedVisualStudioTheme(); }},
        {Theme::SoftCherry, []() { applySoftCherryTheme(); }},
        {Theme::SonicRiders, []() { applySonicRidersTheme(); }},
        {Theme::Unreal, []() { applyUnrealTheme(); }},
        {Theme::VisualStudio, []() { applyVisualStudioTheme(); }},
        {Theme::Windark, []() { applyWindarkTheme(); }},
        {Theme::LedSynthmaster, []() { applyLedSynthmasterTheme(); }}
    };
}
