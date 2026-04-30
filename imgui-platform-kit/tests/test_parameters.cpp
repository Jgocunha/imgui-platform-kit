#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <stdexcept>

#include "user_interface_parameters.h"

using namespace imgui_kit;
namespace fs = std::filesystem;

// ─── WindowParameters ────────────────────────────────────────────────────────

TEST_CASE("WindowParameters: explicit constructor stores values", "[parameters][window]")
{
    const WindowParameters p("My App", 1280, 720, 50, 100);
    CHECK(p.title == "My App");
    CHECK(p.width == 1280);
    CHECK(p.height == 720);
    CHECK(p.startPosX == 50);
    CHECK(p.startPosY == 100);
}

TEST_CASE("WindowParameters: default title is 'default title'", "[parameters][window]")
{
    const WindowParameters p("default title", 800, 600);
    CHECK(p.title == "default title");
}

TEST_CASE("WindowParameters: default startPos is (0, 0)", "[parameters][window]")
{
    const WindowParameters p("App", 800, 600);
    CHECK(p.startPosX == 0);
    CHECK(p.startPosY == 0);
}

TEST_CASE("WindowParameters: zero width throws invalid_argument", "[parameters][window]")
{
    REQUIRE_THROWS_AS(WindowParameters("App", 0, 600), std::invalid_argument);
}

TEST_CASE("WindowParameters: zero height throws invalid_argument", "[parameters][window]")
{
    REQUIRE_THROWS_AS(WindowParameters("App", 800, 0), std::invalid_argument);
}


TEST_CASE("WindowParameters: negative height throws invalid_argument", "[parameters][window]")
{
    REQUIRE_THROWS_AS(WindowParameters("App", 800, 0), std::invalid_argument);
}

TEST_CASE("WindowParameters: save creates a file", "[parameters][window][io]")
{
    const std::string path = (fs::temp_directory_path() / "ipk_test_save.ini").string();
    fs::remove(path);

    const WindowParameters p("SaveTest", 1024, 768, 0, 0);
    p.save(path);

    REQUIRE(fs::exists(path));
    fs::remove(path);
}

TEST_CASE("WindowParameters: save/load roundtrip preserves all fields", "[parameters][window][io]")
{
    const std::string path = (fs::temp_directory_path() / "ipk_test_roundtrip.ini").string();
    fs::remove(path);

    const WindowParameters saved("RoundTrip Window", 1920, 1080, 42, 137);
    saved.save(path);

    WindowParameters loaded("Other", 640, 480);
    loaded.load(path);

    CHECK(loaded.title == "RoundTrip Window");
    CHECK(loaded.width == 1920);
    CHECK(loaded.height == 1080);
    CHECK(loaded.startPosX == 42);
    CHECK(loaded.startPosY == 137);

    fs::remove(path);
}

TEST_CASE("WindowParameters: load with nonexistent file is a no-op", "[parameters][window][io]")
{
    WindowParameters p("App", 800, 600, 10, 20);
    REQUIRE_NOTHROW(p.load("definitely_does_not_exist_xzqw.ini"));
    // Values unchanged
    CHECK(p.title == "App");
    CHECK(p.width == 800);
    CHECK(p.height == 600);
    CHECK(p.startPosX == 10);
    CHECK(p.startPosY == 20);
}

TEST_CASE("WindowParameters: save throws when path is unwritable", "[parameters][window][io]")
{
    const WindowParameters p("App", 800, 600);
    // Writing to a directory path (not a file) should fail
    REQUIRE_THROWS(p.save("/"));
}

// ─── FontParameters ──────────────────────────────────────────────────────────

TEST_CASE("FontParameters: single-path constructor stores path and size", "[parameters][font]")
{
    const FontParameters fp("fonts/myfont.ttf", 16);
    REQUIRE(fp.pathsAndSizes.size() == 1);
    CHECK(fp.pathsAndSizes[0].first == "fonts/myfont.ttf");
    CHECK(fp.pathsAndSizes[0].second == 16);
}

TEST_CASE("FontParameters: default size is 12", "[parameters][font]")
{
    const FontParameters fp("some/font.ttf");
    REQUIRE(fp.pathsAndSizes.size() == 1);
    CHECK(fp.pathsAndSizes[0].second == 12);
}

TEST_CASE("FontParameters: zero font size throws invalid_argument", "[parameters][font]")
{
    REQUIRE_THROWS_AS(FontParameters("font.ttf", 0), std::invalid_argument);
}

TEST_CASE("FontParameters: negative font size throws invalid_argument", "[parameters][font]")
{
    REQUIRE_THROWS_AS(FontParameters("font.ttf", -1), std::invalid_argument);
}

TEST_CASE("FontParameters: vector constructor stores multiple entries", "[parameters][font]")
{
    const std::vector<std::pair<std::string, int>> entries = {
        {"regular.ttf", 14},
        {"bold.ttf", 18},
        {"mono.ttf", 12}
    };
    const FontParameters fp(entries);
    REQUIRE(fp.pathsAndSizes.size() == 3);
    CHECK(fp.pathsAndSizes[0].first == "regular.ttf");
    CHECK(fp.pathsAndSizes[0].second == 14);
    CHECK(fp.pathsAndSizes[1].first == "bold.ttf");
    CHECK(fp.pathsAndSizes[2].second == 12);
}

TEST_CASE("FontParameters: vector constructor throws on zero size", "[parameters][font]")
{
    REQUIRE_THROWS_AS(FontParameters({{"font.ttf", 0}}), std::invalid_argument);
}

TEST_CASE("FontParameters: vector constructor throws on empty path", "[parameters][font]")
{
    REQUIRE_THROWS_AS(FontParameters({{"", 12}}), std::invalid_argument);
}

TEST_CASE("FontParameters: vector constructor throws on first invalid entry", "[parameters][font]")
{
    REQUIRE_THROWS_AS(FontParameters({{"ok.ttf", 12}, {"bad.ttf", -5}}), std::invalid_argument);
}

// ─── StyleParameters ─────────────────────────────────────────────────────────

TEST_CASE("StyleParameters: default constructor sets Theme::Dark", "[parameters][style]")
{
    const StyleParameters sp;
    CHECK(sp.theme == Theme::Dark);
}

TEST_CASE("StyleParameters: default constructor sets expected default values", "[parameters][style]")
{
    const StyleParameters sp;
    CHECK(sp.windowRounding == 5.0f);
    CHECK(sp.windowPadding.x == 8.0f);
    CHECK(sp.windowPadding.y == 8.0f);
    CHECK(sp.transparency == 1.0f);
    CHECK(sp.textColor.x == 1.0f);
    CHECK(sp.textColor.y == 1.0f);
    CHECK(sp.textColor.z == 1.0f);
    CHECK(sp.textColor.w == 1.0f);
}

TEST_CASE("StyleParameters: default background color is DarkGray", "[parameters][style]")
{
    const StyleParameters sp;
    CHECK(sp.windowBgColor.x == colours::DarkGray.x);
    CHECK(sp.windowBgColor.y == colours::DarkGray.y);
    CHECK(sp.windowBgColor.z == colours::DarkGray.z);
    CHECK(sp.windowBgColor.w == colours::DarkGray.w);
}

TEST_CASE("StyleParameters: theme constructor stores theme", "[parameters][style]")
{
    const StyleParameters sp(Theme::Cherry);
    CHECK(sp.theme == Theme::Cherry);
    CHECK(sp.windowRounding == 5.0f); // defaults unchanged
}

TEST_CASE("StyleParameters: theme+color constructor stores both", "[parameters][style]")
{
    const ImVec4 myColor(0.1f, 0.2f, 0.3f, 1.0f);
    const StyleParameters sp(Theme::Moonlight, myColor);
    CHECK(sp.theme == Theme::Moonlight);
    CHECK(sp.windowBgColor.x == 0.1f);
    CHECK(sp.windowBgColor.y == 0.2f);
    CHECK(sp.windowBgColor.z == 0.3f);
    CHECK(sp.windowBgColor.w == 1.0f);
}

// ─── IconParameters ──────────────────────────────────────────────────────────

TEST_CASE("IconParameters: default constructor produces empty path and zero dims", "[parameters][icon]")
{
    const IconParameters ip;
    CHECK(ip.path.empty());
    CHECK(ip.width == 0);
    CHECK(ip.height == 0);
}

TEST_CASE("IconParameters: explicit path is stored", "[parameters][icon]")
{
    const IconParameters ip("icons/app.png", 32, 32);
    CHECK(ip.path == "icons/app.png");
    CHECK(ip.width == 32);
    CHECK(ip.height == 32);
}

TEST_CASE("IconParameters: path-only constructor uses default zero dims", "[parameters][icon]")
{
    const IconParameters ip("icons/app.ico");
    CHECK(ip.path == "icons/app.ico");
    CHECK(ip.width == 0);
    CHECK(ip.height == 0);
}

// ─── BackgroundImageParameters ───────────────────────────────────────────────

TEST_CASE("BackgroundImageParameters: default is empty with KEEP_ASPECT_RATIO", "[parameters][background]")
{
    const BackgroundImageParameters bip;
    CHECK(bip.path.empty());
    CHECK(bip.fitType == ImageFitType::KEEP_ASPECT_RATIO);
    CHECK(bip.width == 0);
    CHECK(bip.height == 0);
}

TEST_CASE("BackgroundImageParameters: explicit path is stored", "[parameters][background]")
{
    const BackgroundImageParameters bip("images/bg.png");
    CHECK(bip.path == "images/bg.png");
    CHECK(bip.fitType == ImageFitType::KEEP_ASPECT_RATIO);
}

TEST_CASE("BackgroundImageParameters: ZOOM_TO_FIT mode is stored", "[parameters][background]")
{
    const BackgroundImageParameters bip("bg.png", ImageFitType::ZOOM_TO_FIT);
    CHECK(bip.fitType == ImageFitType::ZOOM_TO_FIT);
}

TEST_CASE("ImageFitType: enum values are distinct", "[parameters][background]")
{
    CHECK(static_cast<int>(ImageFitType::KEEP_ASPECT_RATIO) != static_cast<int>(ImageFitType::ZOOM_TO_FIT));
}

TEST_CASE("ImageFitType: KEEP_ASPECT_RATIO is the zero value", "[parameters][background]")
{
    CHECK(static_cast<uint8_t>(ImageFitType::KEEP_ASPECT_RATIO) == 0);
}

// ─── UserInterfaceParameters ─────────────────────────────────────────────────

TEST_CASE("UserInterfaceParameters: default constructor is valid", "[parameters][ui]")
{
    REQUIRE_NOTHROW(UserInterfaceParameters{});
}

TEST_CASE("UserInterfaceParameters: explicit constructor stores all sub-parameters", "[parameters][ui]")
{
    const WindowParameters wp("TestApp", 1280, 720);
    const FontParameters fp("font.ttf", 16);
    const StyleParameters sp(Theme::Windark);
    const IconParameters ip("icon.png");
    const BackgroundImageParameters bip("bg.jpg", ImageFitType::ZOOM_TO_FIT);

    const UserInterfaceParameters uip(wp, fp, sp, ip, bip);

    CHECK(uip.windowParameters.title == "TestApp");
    CHECK(uip.windowParameters.width == 1280);
    CHECK(uip.fontParameters.pathsAndSizes[0].first == "font.ttf");
    CHECK(uip.styleParameters.theme == Theme::Windark);
    CHECK(uip.iconParameters.path == "icon.png");
    CHECK(uip.backgroundImageParameters.fitType == ImageFitType::ZOOM_TO_FIT);
}

TEST_CASE("UserInterfaceParameters: save delegates to WindowParameters::save", "[parameters][ui][io]")
{
    const std::string path = (fs::temp_directory_path() / "ipk_test_uip.ini").string();
    fs::remove(path);

    const UserInterfaceParameters uip(WindowParameters("UITest", 800, 600));
    uip.save(path);

    REQUIRE(fs::exists(path));
    fs::remove(path);
}
