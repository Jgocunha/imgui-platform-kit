#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "test_helpers.h"
#include "user_interface_parameters.h"
#include "themes.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

static const std::vector<Theme> ALL_THEMES = {
    Theme::Light, Theme::Dark, Theme::Classic, Theme::AdobeInspired, Theme::BlackDevil,
    Theme::BootstrapDark, Theme::Cherry, Theme::ClassicSteam, Theme::CleanDarkRed, Theme::Comfy,
    Theme::DuckRed, Theme::Darcula, Theme::DarkRuda, Theme::Darky, Theme::DeepDark,
    Theme::DiscordDark, Theme::EnemyMouse, Theme::EverForest, Theme::FutureDark, Theme::Gold,
    Theme::GreenFont, Theme::GreenLeaf, Theme::HazyDark, Theme::MaterialFlat, Theme::Microsoft,
    Theme::Modern, Theme::Moonlight, Theme::Photoshoop, Theme::PurpleComfy, Theme::QuickMinimal,
    Theme::RedFont, Theme::RedOni, Theme::RoundedVisualStudio, Theme::SoftCherry,
    Theme::SonicRiders, Theme::Unreal, Theme::VisualStudio, Theme::Windark,
    Theme::LedSynthmaster,
};

// apply() dispatches to the matching themes[] entry

TEST_CASE("StyleParameters: apply() applies the selected theme then style fields", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    for (const Theme theme : ALL_THEMES)
    {
        INFO("Theme enum value: " << static_cast<int>(theme));

        StyleParameters style(theme);
        style.windowPadding = ImVec2(12.0f, 14.0f);
        style.framePadding = ImVec2(6.0f, 5.0f);
        style.windowRounding = 7.5f;
        style.transparency = 0.75f;
        style.windowBgColor = ImVec4(0.11f, 0.22f, 0.33f, 1.0f);
        style.textColor = ImVec4(0.9f, 0.8f, 0.7f, 1.0f);

        ImGui::GetStyle() = ImGuiStyle{};
        style.apply();
        // Copy by value before any later reset of the global style.
        const ImGuiStyle applied = ImGui::GetStyle();

        CHECK(applied.WindowPadding.x == 12.0f);
        CHECK(applied.WindowPadding.y == 14.0f);
        CHECK(applied.FramePadding.x == 6.0f);
        CHECK(applied.FramePadding.y == 5.0f);
        CHECK(applied.WindowRounding == 7.5f);
        CHECK(applied.Alpha == 0.75f);
        CHECK(colorsEqual(applied.Colors[ImGuiCol_WindowBg], style.windowBgColor));
        CHECK(colorsEqual(applied.Colors[ImGuiCol_Text], style.textColor));

        // Non-overridden theme colours still come from the theme table.
        ImGui::GetStyle() = ImGuiStyle{};
        themes.at(theme)();
        const ImVec4 direct_FrameBg = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
        CHECK(colorsEqual(applied.Colors[ImGuiCol_FrameBg], direct_FrameBg));
    }
}

TEST_CASE("StyleParameters: default-constructed params apply Dark theme plus default fields", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    const StyleParameters style;
    REQUIRE(style.theme == Theme::Dark);

    ImGui::GetStyle() = ImGuiStyle{};
    style.apply();
    const ImGuiStyle applied = ImGui::GetStyle();

    CHECK(applied.WindowRounding == style.windowRounding);
    CHECK(applied.WindowPadding.x == style.windowPadding.x);
    CHECK(applied.WindowPadding.y == style.windowPadding.y);
    CHECK(applied.FramePadding.x == style.framePadding.x);
    CHECK(applied.FramePadding.y == style.framePadding.y);
    CHECK(applied.Alpha == style.transparency);
    CHECK(colorsEqual(applied.Colors[ImGuiCol_WindowBg], style.windowBgColor));
    CHECK(colorsEqual(applied.Colors[ImGuiCol_Text], style.textColor));
}

TEST_CASE("StyleParameters: apply() forwards non-theme fields after theme", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    StyleParameters style(Theme::Dark);
    style.windowBgColor = ImVec4(0.123f, 0.456f, 0.789f, 1.0f);
    style.windowPadding = ImVec2(99.0f, 77.0f);
    style.framePadding = ImVec2(15.0f, 9.0f);
    style.windowRounding = 42.0f;
    style.transparency = 0.5f;
    style.textColor = ImVec4(0.9f, 0.1f, 0.2f, 1.0f);

    ImGui::GetStyle() = ImGuiStyle{};
    style.apply();
    const ImGuiStyle applied = ImGui::GetStyle();

    CHECK(applied.WindowPadding.x == 99.0f);
    CHECK(applied.WindowPadding.y == 77.0f);
    CHECK(applied.FramePadding.x == 15.0f);
    CHECK(applied.FramePadding.y == 9.0f);
    CHECK(applied.WindowRounding == 42.0f);
    CHECK(applied.Alpha == 0.5f);
    CHECK(colorsEqual(applied.Colors[ImGuiCol_Text], style.textColor));
    CHECK(colorsEqual(applied.Colors[ImGuiCol_WindowBg], style.windowBgColor));
}

TEST_CASE("StyleParameters: apply() is const and repeatable", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    const StyleParameters style(Theme::Cherry);

    ImGui::GetStyle() = ImGuiStyle{};
    style.apply();
    const ImVec4 first = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    style.apply();
    const ImVec4 second = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    CHECK(colorsEqual(first, second));
}

TEST_CASE("StyleParameters: stored fields are unchanged by apply()", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    StyleParameters style(Theme::Gold, ImVec4(0.1f, 0.2f, 0.3f, 1.0f));
    style.windowPadding = ImVec2(11.0f, 22.0f);
    style.framePadding = ImVec2(3.0f, 4.0f);
    style.windowRounding = 3.5f;
    style.transparency = 0.8f;
    style.textColor = ImVec4(0.4f, 0.5f, 0.6f, 1.0f);

    style.apply();

    CHECK(style.theme == Theme::Gold);
    CHECK(colorsEqual(style.windowBgColor, ImVec4(0.1f, 0.2f, 0.3f, 1.0f)));
    CHECK(style.windowPadding.x == 11.0f);
    CHECK(style.windowPadding.y == 22.0f);
    CHECK(style.framePadding.x == 3.0f);
    CHECK(style.framePadding.y == 4.0f);
    CHECK(style.windowRounding == 3.5f);
    CHECK(style.transparency == 0.8f);
    CHECK(colorsEqual(style.textColor, ImVec4(0.4f, 0.5f, 0.6f, 1.0f)));
}