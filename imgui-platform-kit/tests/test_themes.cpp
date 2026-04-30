#include <catch2/catch_test_macros.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

#include "themes.h"

using namespace imgui_kit;

// Complete list of all Theme enum values — update this if new themes are added.
static const std::vector<Theme> ALL_THEMES = {
    Theme::Light,
    Theme::Dark,
    Theme::Classic,
    Theme::AdobeInspired,
    Theme::BlackDevil,
    Theme::BootstrapDark,
    Theme::Cherry,
    Theme::ClassicSteam,
    Theme::CleanDarkRed,
    Theme::Comfy,
    Theme::DuckRed,
    Theme::Darcula,
    Theme::DarkRuda,
    Theme::Darky,
    Theme::DeepDark,
    Theme::DiscordDark,
    Theme::EnemyMouse,
    Theme::EverForest,
    Theme::FutureDark,
    Theme::Gold,
    Theme::GreenFont,
    Theme::GreenLeaf,
    Theme::HazyDark,
    Theme::MaterialFlat,
    Theme::Microsoft,
    Theme::Modern,
    Theme::Moonlight,
    Theme::Photoshoop,
    Theme::PurpleComfy,
    Theme::QuickMinimal,
    Theme::RedFont,
    Theme::RedOni,
    Theme::RoundedVisualStudio,
    Theme::SoftCherry,
    Theme::SonicRiders,
    Theme::Unreal,
    Theme::VisualStudio,
    Theme::Windark,
    Theme::LedSynthmaster,
};

static constexpr int EXPECTED_THEME_COUNT = 39;

// ─── Theme count ─────────────────────────────────────────────────────────────

TEST_CASE("Themes: there are exactly 39 theme enum values", "[themes]")
{
    REQUIRE(static_cast<int>(ALL_THEMES.size()) == EXPECTED_THEME_COUNT);
}

// ─── themeNames map ───────────────────────────────────────────────────────────

TEST_CASE("Themes: themeNames has an entry for every Theme enum value", "[themes][names]")
{
    for (const Theme theme : ALL_THEMES)
    {
        INFO("Missing theme with enum value " << static_cast<int>(theme));
        REQUIRE(themeNames.count(theme) == 1);
    }
}

TEST_CASE("Themes: themeNames has no extra entries", "[themes][names]")
{
    REQUIRE(themeNames.size() == EXPECTED_THEME_COUNT);
}

TEST_CASE("Themes: themeNames entries are non-empty strings", "[themes][names]")
{
    for (const auto& [theme, name] : themeNames)
    {
        INFO("Empty name for theme " << static_cast<int>(theme));
        REQUIRE_FALSE(name.empty());
    }
}

TEST_CASE("Themes: themeNames entries are unique", "[themes][names]")
{
    std::unordered_set<std::string> seen;
    for (const auto& [theme, name] : themeNames)
    {
        INFO("Duplicate theme name: " << name);
        REQUIRE(seen.find(name) == seen.end());
        seen.insert(name);
    }
}

TEST_CASE("Themes: specific themeNames values are correct", "[themes][names]")
{
    CHECK(themeNames.at(Theme::Light) == "Light");
    CHECK(themeNames.at(Theme::Dark) == "Dark");
    CHECK(themeNames.at(Theme::Classic) == "Classic");
    CHECK(themeNames.at(Theme::Windark) == "Windark");
    CHECK(themeNames.at(Theme::DiscordDark) == "Discord Dark");
    CHECK(themeNames.at(Theme::MaterialFlat) == "Material Flat");
    CHECK(themeNames.at(Theme::RedOni) == "Red Oni");
    CHECK(themeNames.at(Theme::LedSynthmaster) == "Led Synthmaster");
}

// ─── themes (function) map ────────────────────────────────────────────────────

TEST_CASE("Themes: themes map has an entry for every Theme enum value", "[themes][map]")
{
    for (const Theme theme : ALL_THEMES)
    {
        INFO("Missing themes entry for enum value " << static_cast<int>(theme));
        REQUIRE(themes.count(theme) == 1);
    }
}

TEST_CASE("Themes: themes map has no extra entries", "[themes][map]")
{
    REQUIRE(themes.size() == EXPECTED_THEME_COUNT);
}

TEST_CASE("Themes: every themes entry holds a callable function", "[themes][map]")
{
    for (const Theme theme : ALL_THEMES)
    {
        INFO("Null function for theme " << static_cast<int>(theme));
        REQUIRE(static_cast<bool>(themes.at(theme)));
    }
}

// ─── themeNames and themes are consistent ────────────────────────────────────

TEST_CASE("Themes: themeNames and themes have the same keys", "[themes]")
{
    for (const auto& [theme, name] : themeNames)
    {
        INFO("themeNames key " << name << " missing from themes map");
        REQUIRE(themes.count(theme) == 1);
    }

    for (const auto& [theme, fn] : themes)
    {
        INFO("themes key " << static_cast<int>(theme) << " missing from themeNames map");
        REQUIRE(themeNames.count(theme) == 1);
    }
}
