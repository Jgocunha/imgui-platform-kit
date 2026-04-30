#include <catch2/catch_test_macros.hpp>
#include "user_interface_window.h"

using namespace imgui_kit;

// Reset helper — call at the start of each test that depends on a known state.
static void resetFlags() { setGlobalWindowFlags(ImGuiWindowFlags_None); }

// ─── getGlobalWindowFlags / setGlobalWindowFlags ─────────────────────────────

TEST_CASE("GlobalWindowFlags: initial state is None", "[flags]")
{
    resetFlags();
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_None);
}

TEST_CASE("GlobalWindowFlags: setGlobalWindowFlags stores the exact value", "[flags]")
{
    resetFlags();
    setGlobalWindowFlags(ImGuiWindowFlags_NoResize);
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_NoResize);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: setGlobalWindowFlags replaces previous value", "[flags]")
{
    setGlobalWindowFlags(ImGuiWindowFlags_NoMove);
    setGlobalWindowFlags(ImGuiWindowFlags_NoTitleBar);
    CHECK(getGlobalWindowFlags() == ImGuiWindowFlags_NoTitleBar);
    CHECK((getGlobalWindowFlags() & ImGuiWindowFlags_NoMove) == 0);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: setting None clears all flags", "[flags]")
{
    setGlobalWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    setGlobalWindowFlags(ImGuiWindowFlags_None);
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_None);
}

// ─── addGlobalWindowFlags ────────────────────────────────────────────────────

TEST_CASE("GlobalWindowFlags: addGlobalWindowFlags ORs the flag in", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoResize);
    REQUIRE((getGlobalWindowFlags() & ImGuiWindowFlags_NoResize) != 0);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: addGlobalWindowFlags preserves existing flags", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoResize);
    addGlobalWindowFlags(ImGuiWindowFlags_NoMove);
    CHECK((getGlobalWindowFlags() & ImGuiWindowFlags_NoResize) != 0);
    CHECK((getGlobalWindowFlags() & ImGuiWindowFlags_NoMove) != 0);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: adding the same flag twice is idempotent", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);
    addGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_NoCollapse);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: addGlobalWindowFlags with None is a no-op", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoResize);
    const ImGuiWindowFlags before = getGlobalWindowFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_None);
    REQUIRE(getGlobalWindowFlags() == before);
    resetFlags();
}

// ─── removeGlobalWindowFlags ─────────────────────────────────────────────────

TEST_CASE("GlobalWindowFlags: removeGlobalWindowFlags clears a flag", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    removeGlobalWindowFlags(ImGuiWindowFlags_NoResize);
    CHECK((getGlobalWindowFlags() & ImGuiWindowFlags_NoResize) == 0);
    CHECK((getGlobalWindowFlags() & ImGuiWindowFlags_NoMove) != 0);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: removing a flag that is not set is a no-op", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoMove);
    removeGlobalWindowFlags(ImGuiWindowFlags_NoResize); // not set
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_NoMove);
    resetFlags();
}

TEST_CASE("GlobalWindowFlags: removing all flags leaves None", "[flags]")
{
    resetFlags();
    const ImGuiWindowFlags allThree = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
    setGlobalWindowFlags(allThree);
    removeGlobalWindowFlags(allThree);
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_None);
}

// ─── Combined operations ─────────────────────────────────────────────────────

TEST_CASE("GlobalWindowFlags: add then remove returns to original state", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);
    removeGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);
    REQUIRE(getGlobalWindowFlags() == ImGuiWindowFlags_None);
}

TEST_CASE("GlobalWindowFlags: multiple flags combined correctly", "[flags]")
{
    resetFlags();
    addGlobalWindowFlags(ImGuiWindowFlags_NoResize);
    addGlobalWindowFlags(ImGuiWindowFlags_NoMove);
    addGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);

    const ImGuiWindowFlags current = getGlobalWindowFlags();
    CHECK((current & ImGuiWindowFlags_NoResize) != 0);
    CHECK((current & ImGuiWindowFlags_NoMove) != 0);
    CHECK((current & ImGuiWindowFlags_NoCollapse) != 0);
    CHECK((current & ImGuiWindowFlags_NoTitleBar) == 0);

    resetFlags();
}

TEST_CASE("GlobalWindowFlags: setGlobalWindowFlags with multiple flags at once", "[flags]")
{
    resetFlags();
    setGlobalWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    const ImGuiWindowFlags current = getGlobalWindowFlags();
    CHECK((current & ImGuiWindowFlags_NoResize) != 0);
    CHECK((current & ImGuiWindowFlags_NoScrollbar) != 0);
    CHECK((current & ImGuiWindowFlags_NoMove) == 0);
    resetFlags();
}
