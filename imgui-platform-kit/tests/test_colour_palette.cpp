#include <catch2/catch_test_macros.hpp>
#include "colour_palette.h"

using namespace imgui_kit::colours;

// Helper: check all four RGBA components
static void checkColor(const ImVec4& c, float r, float g, float b, float a)
{
    CHECK(c.x == r);
    CHECK(c.y == g);
    CHECK(c.z == b);
    CHECK(c.w == a);
}

// ─── Every colour has alpha = 1.0 ────────────────────────────────────────────

TEST_CASE("Colours: all colours have alpha = 1.0f", "[colours]")
{
    // Basic
    CHECK(White.w == 1.0f);
    CHECK(Black.w == 1.0f);
    CHECK(Red.w == 1.0f);
    CHECK(Green.w == 1.0f);
    CHECK(Blue.w == 1.0f);
    CHECK(Yellow.w == 1.0f);
    CHECK(Orange.w == 1.0f);
    CHECK(Purple.w == 1.0f);
    CHECK(Cyan.w == 1.0f);
    CHECK(Magenta.w == 1.0f);
    // Extended
    CHECK(Coral.w == 1.0f);
    CHECK(Violet.w == 1.0f);
    CHECK(Gold.w == 1.0f);
    CHECK(Azure.w == 1.0f);
    CHECK(Beige.w == 1.0f);
    CHECK(Lime.w == 1.0f);
    CHECK(Teal.w == 1.0f);
    CHECK(Olive.w == 1.0f);
    CHECK(Maroon.w == 1.0f);
    CHECK(Navy.w == 1.0f);
    CHECK(Mint.w == 1.0f);
    CHECK(Peach.w == 1.0f);
    // Grayscales
    CHECK(DarkGray.w == 1.0f);
    CHECK(Gray.w == 1.0f);
    CHECK(LightGray.w == 1.0f);
    CHECK(Silver.w == 1.0f);
    // Pastels
    CHECK(PastelPink.w == 1.0f);
    CHECK(PastelGreen.w == 1.0f);
    CHECK(PastelBlue.w == 1.0f);
    CHECK(PastelYellow.w == 1.0f);
    CHECK(PastelOrange.w == 1.0f);
    CHECK(PastelViolet.w == 1.0f);
    // Dark
    CHECK(DarkRed.w == 1.0f);
    CHECK(DarkGreen.w == 1.0f);
    CHECK(DarkBlue.w == 1.0f);
    CHECK(DarkYellow.w == 1.0f);
    CHECK(DarkPurple.w == 1.0f);
    CHECK(DarkCyan.w == 1.0f);
    CHECK(DarkMagenta.w == 1.0f);
    CHECK(DarkOrange.w == 1.0f);
}

// ─── Basic / Primary Colors ───────────────────────────────────────────────────

TEST_CASE("Colours: White is (1, 1, 1, 1)", "[colours][basic]")
{
    checkColor(White, 1.0f, 1.0f, 1.0f, 1.0f);
}

TEST_CASE("Colours: Black is (0, 0, 0, 1)", "[colours][basic]")
{
    checkColor(Black, 0.0f, 0.0f, 0.0f, 1.0f);
}

TEST_CASE("Colours: Red is pure red (1, 0, 0, 1)", "[colours][basic]")
{
    checkColor(Red, 1.0f, 0.0f, 0.0f, 1.0f);
}

TEST_CASE("Colours: Green is pure green (0, 1, 0, 1)", "[colours][basic]")
{
    checkColor(Green, 0.0f, 1.0f, 0.0f, 1.0f);
}

TEST_CASE("Colours: Blue is pure blue (0, 0, 1, 1)", "[colours][basic]")
{
    checkColor(Blue, 0.0f, 0.0f, 1.0f, 1.0f);
}

TEST_CASE("Colours: Yellow is (1, 1, 0, 1)", "[colours][basic]")
{
    checkColor(Yellow, 1.0f, 1.0f, 0.0f, 1.0f);
}

TEST_CASE("Colours: Cyan is (0, 1, 1, 1)", "[colours][basic]")
{
    checkColor(Cyan, 0.0f, 1.0f, 1.0f, 1.0f);
}

TEST_CASE("Colours: Magenta is (1, 0, 1, 1)", "[colours][basic]")
{
    checkColor(Magenta, 1.0f, 0.0f, 1.0f, 1.0f);
}

TEST_CASE("Colours: Orange has full red, partial green, no blue", "[colours][basic]")
{
    CHECK(Orange.x == 1.0f);
    CHECK(Orange.y == 0.65f);
    CHECK(Orange.z == 0.0f);
}

TEST_CASE("Colours: Purple has equal red and blue, no green", "[colours][basic]")
{
    CHECK(Purple.x == 0.50f);
    CHECK(Purple.y == 0.0f);
    CHECK(Purple.z == 0.50f);
}

// ─── Extended palette spot-checks ────────────────────────────────────────────

TEST_CASE("Colours: Teal has no red, equal green and blue at 0.5", "[colours][extended]")
{
    CHECK(Teal.x == 0.0f);
    CHECK(Teal.y == 0.50f);
    CHECK(Teal.z == 0.50f);
}

TEST_CASE("Colours: Olive has equal red and green at 0.5, no blue", "[colours][extended]")
{
    CHECK(Olive.x == 0.50f);
    CHECK(Olive.y == 0.50f);
    CHECK(Olive.z == 0.0f);
}

TEST_CASE("Colours: Navy has no red, no green, half blue", "[colours][extended]")
{
    CHECK(Navy.x == 0.0f);
    CHECK(Navy.y == 0.0f);
    CHECK(Navy.z == 0.50f);
}

TEST_CASE("Colours: Maroon has half red, no green, no blue", "[colours][extended]")
{
    CHECK(Maroon.x == 0.50f);
    CHECK(Maroon.y == 0.0f);
    CHECK(Maroon.z == 0.0f);
}

// ─── Grayscales ───────────────────────────────────────────────────────────────

TEST_CASE("Colours: DarkGray has equal R=G=B=0.20", "[colours][grayscale]")
{
    CHECK(DarkGray.x == 0.20f);
    CHECK(DarkGray.y == 0.20f);
    CHECK(DarkGray.z == 0.20f);
}

TEST_CASE("Colours: Gray has equal R=G=B=0.50", "[colours][grayscale]")
{
    CHECK(Gray.x == 0.50f);
    CHECK(Gray.y == 0.50f);
    CHECK(Gray.z == 0.50f);
}

TEST_CASE("Colours: LightGray has equal R=G=B=0.75", "[colours][grayscale]")
{
    CHECK(LightGray.x == 0.75f);
    CHECK(LightGray.y == 0.75f);
    CHECK(LightGray.z == 0.75f);
}

TEST_CASE("Colours: grayscales are ordered DarkGray < Gray < LightGray", "[colours][grayscale]")
{
    CHECK(DarkGray.x < Gray.x);
    CHECK(Gray.x < LightGray.x);
}

TEST_CASE("Colours: Silver is slightly blue-shifted compared to LightGray", "[colours][grayscale]")
{
    CHECK(Silver.x == 0.75f);
    CHECK(Silver.y == 0.75f);
    CHECK(Silver.z == 0.80f); // blue channel slightly higher
}

// ─── Dark variants are darker than their counterparts ────────────────────────

TEST_CASE("Colours: DarkRed is darker than Red", "[colours][dark]")
{
    CHECK(DarkRed.x < Red.x);
    CHECK(DarkRed.y == 0.0f);
    CHECK(DarkRed.z == 0.0f);
}

TEST_CASE("Colours: DarkGreen is darker than Green", "[colours][dark]")
{
    CHECK(DarkGreen.y < Green.y);
    CHECK(DarkGreen.x == 0.0f);
    CHECK(DarkGreen.z == 0.0f);
}

TEST_CASE("Colours: DarkBlue is darker than Blue", "[colours][dark]")
{
    CHECK(DarkBlue.z < Blue.z);
    CHECK(DarkBlue.x == 0.0f);
    CHECK(DarkBlue.y == 0.0f);
}

TEST_CASE("Colours: DarkCyan channels are consistent (equal R=0, G=B equal)", "[colours][dark]")
{
    CHECK(DarkCyan.x == 0.0f);
    CHECK(DarkCyan.y == DarkCyan.z);
}

TEST_CASE("Colours: DarkMagenta channels are consistent (R=B equal, G=0)", "[colours][dark]")
{
    CHECK(DarkMagenta.y == 0.0f);
    CHECK(DarkMagenta.x == DarkMagenta.z);
}

// ─── Pastel colors have higher brightness than their base ────────────────────

TEST_CASE("Colours: PastelPink has high red channel", "[colours][pastel]")
{
    CHECK(PastelPink.x == 1.0f);
    CHECK(PastelPink.y > 0.5f);
    CHECK(PastelPink.z > 0.5f);
}

TEST_CASE("Colours: PastelGreen has high green channel", "[colours][pastel]")
{
    CHECK(PastelGreen.y > PastelGreen.x);
    CHECK(PastelGreen.y > PastelGreen.z);
}

TEST_CASE("Colours: PastelBlue has high blue channel", "[colours][pastel]")
{
    CHECK(PastelBlue.z > PastelBlue.x);
}
